#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <stdbool.h>

#include "mytbf.h"

#define CPS         10
#define BUF_SIZE    1000
#define BURST       BUF_SIZE

static void clear_src_fd(int s, void *arg)
{
    int *ptr = arg;
    close(*ptr);
}

static void clear_tbf(int s, void *arg)
{
    tbf_destroy(&arg);
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
    tbf_t *mytbf = NULL;
    int fetched, returned;

    do
    {
        src_fd = open(argv[1], O_RDONLY);
        if (src_fd < 0 && errno != EINTR)
        {
            perror("open()");
            exit(EXIT_FAILURE);
        }
    } while (src_fd < 0);
    on_exit(clear_src_fd, &src_fd);

    mytbf = tbf_init(CPS, BURST);
    if (mytbf == NULL)
    {
        fprintf(stderr, "tbf_init() failed\n");
        exit(EXIT_FAILURE);
    }
    on_exit(clear_tbf, mytbf);
    
    while (true)
    {
        fetched = tbf_fetch_token(mytbf, BURST);
        if (fetched <= 0)
        {
            fprintf(stderr, "tbf_fetch_token() failed: %s", strerror(-fetched));
            exit(EXIT_FAILURE);
        }

        do
        {
            read_bytes = read(src_fd, buf, fetched);
            
            if (read_bytes < 0 && errno != EINTR)
            {
                perror("read()");
                exit(EXIT_FAILURE);
            }
            if (read_bytes == 0)
            {
                exit(EXIT_SUCCESS);
            }
        } while (read_bytes < 0);

        if (read_bytes < fetched)
        {
            returned = tbf_return_token(mytbf, fetched - read_bytes);
            if (returned <= 0)
            {
                fprintf(stderr, "tbf_return_token() fail: %s", strerror(-returned));
                exit(EXIT_FAILURE);
            }
        }

        offset = 0;
        left = read_bytes;

        do
        {
            write_bytes = write(dst_fd, buf+offset, left);
            if (write_bytes < 0 && errno != EINTR)
            {
                perror("write()");
                exit(EXIT_FAILURE);
            }
            left -= write_bytes;
            offset += write_bytes;
        } while (left > 0);
    }
}