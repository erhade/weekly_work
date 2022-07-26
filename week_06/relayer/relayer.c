#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <limits.h>
#include <pthread.h>
#include <string.h>

#include "relayer.h"

#define MAX_SIZE    1024

enum
{
    ERR_INVALID = INT_MIN,
    ERR_MALLOC,
    ERR_JOBS_SPACE,
};

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
    pthread_t tid;
}fsm_st;

static fsm_st *jobs[JOB_SIZE];
static pthread_mutex_t mut = PTHREAD_MUTEX_INITIALIZER; /* mutex_destory */
// static pthread_once_t once = PTHREAD_ONCE_INIT; 

char *relayer_err_str(int err_no)
{
    switch (err_no)
    {
    case ERR_INVALID:
        return "Invalid parameter";
        break;
    
    case ERR_MALLOC:
        return "malloc failed";
        break;

    case ERR_JOBS_SPACE:
        return "Insufficient space for jobs";
        break;



    default:
        return NULL;
        break;
    }
}

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

    while (fsm1.state != STATE_T && fsm2.state != STATE_T)
    {
        driver(&fsm1);
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

}

static int _find_pos_unlock(void)
{
    for (int i = 0; i < JOB_SIZE; i++)
    {
        if (jobs[i] == NULL)
            return i;
    }
    return -1;
}

static void *jobs_handler(void *arg)
{
    fsm_st *fsm = arg;
    relay(fsm->fd_src, fsm->fd_dst);
    pthread_exit(NULL);
}

static void module_unload(void)
{
    
}

static void module_load(fsm_st *fsm)
{
    int ret;
    pthread_t tid;
    if ((ret = pthread_create(&tid, NULL, jobs_handler, fsm)))
    {
        fprintf(stderr, "pthread_create: %s\n", strerror(ret));
        exit(EXIT_FAILURE);
    }
    fsm->tid = tid;
    // atexit(module_unload);
}

int relayer_init(int fd_src, int fd_dst)
{
    if (fd_src < 0 || fd_dst < 0)
        return ERR_INVALID;
    fsm_st *fsm = malloc(sizeof(*fsm));
    if (fsm == NULL)
        return ERR_MALLOC;
    fsm->fd_src = fd_src;
    fsm->fd_dst = fd_dst;
    fsm->state = STATE_R;
    pthread_mutex_lock(&mut);
    int pos = _find_pos_unlock();
    if (pos == -1)
    {
        pthread_mutex_unlock(&mut);
        return ERR_JOBS_SPACE;
    }
    jobs[pos] = fsm;
    pthread_mutex_unlock(&mut);
    module_load(fsm);
    // pthread_once(&once, module_load);
    return pos;
}

int relayer_destroy(int re_id)
{
    int ret;
    if (re_id < 0 || re_id >= JOB_SIZE)
        return ERR_INVALID;
    pthread_mutex_lock(&mut);
    if (jobs[re_id] == NULL)
    {
        pthread_mutex_unlock(&mut);
        return ERR_INVALID;
    }
    if ((ret = pthread_join(jobs[re_id]->tid, NULL)))
    {
        fprintf(stderr, "pthread_join: %s\n", strerror(ret));
        pthread_mutex_unlock(&mut);
        exit(EXIT_FAILURE);
    }
    free(jobs[re_id]);
    jobs[re_id] = NULL;
    pthread_mutex_unlock(&mut);
    return 0;
}