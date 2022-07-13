#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <stdbool.h>
#include <signal.h>

#define CPS         10
#define BUF_SIZE    CPS

static void alrm_handler(int s)
{
    alarm(1);
}

int main(int argc, char const *argv[])
{
    if (argc < 2)
    {
        fprintf(stderr, "Usage: %s <srcfile>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    char buf[BUF_SIZE];
    int src_fd, dst_fd;
    dst_fd = 1;
    ssize_t read_bytes, write_bytes, offset, left;

    do
    {
        src_fd = open(argv[1], O_RDONLY);
        if (src_fd < 0 && errno != EINTR)
        {
            perror("open()");
            exit(EXIT_FAILURE);
        }
    } while (src_fd < 0);

    signal(SIGALRM, alrm_handler);
    alarm(1);
    
    while (true)
    {
        do
        {
            read_bytes = read(src_fd, buf, BUF_SIZE);
            if (read_bytes < 0 && errno != EINTR)
            {
                perror("read()");
                close(src_fd);
                exit(EXIT_FAILURE);
            }
            if (read_bytes == 0)
            {
                close(src_fd);
                exit(EXIT_SUCCESS);
            }
        } while (read_bytes < 0);

        offset = 0;
        left = read_bytes;

        do
        {
            write_bytes = write(dst_fd, buf+offset, left);
            if (write_bytes < 0 && errno != EINTR)
            {
                perror("write()");
                close(src_fd);
                exit(EXIT_FAILURE);
            }
            left -= write_bytes;
            offset += write_bytes;
        } while (left > 0);
        pause();
    }
    
    close(src_fd);
    exit(EXIT_SUCCESS);
}
