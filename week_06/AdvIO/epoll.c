#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <sys/epoll.h>

#define DATA1    "data1"
#define DATA2    "data2"
#define MAX_SIZE  1024

enum
{
    STATE_R = 1,
    STATE_W,
    STATE_E,
    STATE_T,
};

typedef struct fsm_st
{
    char state;
    int fd_src;
    int fd_dst;
    char buf[MAX_SIZE];
    int rd_len;
    int pos;
    const char *errstr;
}fsm_st;

static void driver(fsm_st *fsm)
{
    int ret;
    switch (fsm->state)
    {
    case STATE_R:
        fsm->rd_len = read(fsm->fd_src, fsm->buf, MAX_SIZE);
        fsm->pos = 0;
        if (fsm->rd_len > 0)
            fsm->state = STATE_W;
        else if (fsm->rd_len < 0 && errno == EAGAIN)
            fsm->state = STATE_R;
        else if (fsm->rd_len < 0 && errno != EAGAIN)
        {
            fsm->state = STATE_E;
            fsm->errstr = "read fail";
        }
        else
            /* 
             * Since the reading and writing is an regular file, 
             * when the file is empty, no error will be reported, 
             * but 0 will be returned. Therefore, in order to read circularly, 
             * the subsequent status is set to STATE_R.
             */
            fsm->state = STATE_R; 
            /* fsm->state = STATE_T; */ 
        break;
    
    case STATE_W:
        ret = write(fsm->fd_dst, fsm->buf+fsm->pos, fsm->rd_len);
        if (ret > 0)
        {
            fsm->rd_len -= ret;
            fsm->pos += ret;
            if (fsm->rd_len > 0)
                fsm->state = STATE_W;
            else
                fsm->state = STATE_R;
        }
        else if (ret < 0 && errno == EAGAIN)
            fsm->state = STATE_W;
        else if (ret < 0 && errno != EAGAIN)
        {
            fsm->state = STATE_E;
            fsm->errstr = "write fail";
        }
        else
            fsm->state = STATE_T;
        break;

    case STATE_E:
        perror(fsm->errstr);
        fsm->state = STATE_T;
        break;

    case STATE_T:
        /* do sth */
        break;

    default:
        abort();
        break;
    }
}

static void relay(int fd1, int fd2)
{
    int fd1_flags, fd2_flags;

    if ((fd1_flags = fcntl(fd1, F_GETFD) == -1))
    {
        perror("fcntl fd1 getfd fail");
        exit(EXIT_FAILURE);
    }

    if ((fcntl(fd1, F_SETFD, fd1_flags | O_NONBLOCK)) == -1)
    {
        perror("fcntl fd1 setfd fail");
        exit(EXIT_FAILURE);
    }

    if ((fd2_flags = fcntl(fd2, F_GETFD)) == -1)
    {
        perror("fcntl fd2 getfd fail");
        exit(EXIT_FAILURE);
    }

    if ((fcntl(fd2, F_SETFD, fd2_flags | O_NONBLOCK)) == -1)
    {
        perror("fcntl fd2 setfd fail");
        exit(EXIT_FAILURE);
    }

    fsm_st fsm1, fsm2;

    fsm1.fd_src = fd1; fsm1.fd_dst = fd2;
    fsm2.fd_src = fd2; fsm2.fd_dst = fd1;
    fsm1.state = STATE_R; fsm2.state = STATE_R;
    
    int epfd = epoll_create(10);

    struct epoll_event ev;
    ev.data.fd = fd1;
    ev.events = 0;
    epoll_ctl(epfd, EPOLL_CTL_ADD, fd1, &ev);

    ev.data.fd = fd2;
    ev.events = 0;
    epoll_ctl(epfd, EPOLL_CTL_ADD, fd2, &ev);

    while (fsm1.state != STATE_T && fsm2.state != STATE_T)
    {
        if (fsm1.state == STATE_E)
        {
            driver(&fsm1);
            continue;
        }
        if (fsm2.state == STATE_E)
        {
            driver(&fsm2);
            continue;
        }

        /* Arrange the monitoring site */
        ev.events = 0;
        ev.data.fd = fd1;
        if (fsm1.state == STATE_R)
            ev.events |= EPOLLIN;
        if (fsm2.state == STATE_W)
            ev.events |= EPOLLOUT;
        epoll_ctl(epfd, EPOLL_CTL_MOD, fd1, &ev);

        ev.events = 0;
        ev.data.fd = fd2;
        if (fsm2.state == STATE_R)
            ev.events |= EPOLLIN;
        if (fsm1.state == STATE_W)
            ev.events |= EPOLLOUT;
        epoll_ctl(epfd, EPOLL_CTL_MOD, fd2, &ev);

        printf("111\n");
        /* monitor */
        /* !!! epoll?????????????????????????????????events????????????????????????????????? */
        while (epoll_wait(epfd, &ev, 1, -1) < 0)
        {
            if (errno == EINTR)
                continue;
            perror("epoll()");
            exit(EXIT_FAILURE);
        }
        printf("222\n");
        /* View monitoring results */
        if ((ev.data.fd == fd1 && ev.events & EPOLLIN) || (ev.data.fd == fd2 && ev.events & EPOLLOUT))
            driver(&fsm1);
        if ((ev.data.fd == fd2 && ev.events & EPOLLIN) || (ev.data.fd == fd1 && ev.events & EPOLLOUT))
            driver(&fsm2);
    }

    if ((fcntl(fd1, F_SETFD, fd1_flags)) == -1)
    {
        perror("fcntl fd1 setfd fail");
        exit(EXIT_FAILURE);
    }

    if ((fcntl(fd2, F_SETFD, fd2_flags)) == -1)
    {
        perror("fcntl fd2 setfd fail");
        exit(EXIT_FAILURE);
    }

    close(epfd);
}

int main(int argc, char const *argv[])
{
    int fd1, fd2;

    if ((fd1 = open(DATA1, O_RDWR | O_CREAT | O_TRUNC, 0666)) < 0)
    {
        perror("open fd1 fail");
        exit(EXIT_FAILURE);
    }

    if ((fd2 = open(DATA2, O_RDWR | O_CREAT | O_TRUNC | O_NONBLOCK, 0666)) < 0)
    {
        perror("open fd2 fail");
        exit(EXIT_FAILURE);
    }

    relay(fd1, fd2);

    close(fd1);
    close(fd2);

    exit(EXIT_SUCCESS);
}
