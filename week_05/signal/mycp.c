#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <stdbool.h>

#define BUF_SIZE    512

int main(int argc, char const *argv[])
{
    if (argc < 3)
    {
        fprintf(stderr, "Usage: %s <srcfile> <dstfile>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    char buf[BUF_SIZE];
    int src_fd, dst_fd;
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

    do
    {
        dst_fd = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0666);
        if (dst_fd < 0 && errno != EINTR)
        {
            perror("open()");
            close(src_fd);
            exit(EXIT_FAILURE);
        }
    } while (dst_fd < 0);
    
    while (true)
    {
        do
        {
            read_bytes = read(src_fd, buf, BUF_SIZE);
            if (read_bytes < 0 && errno != EINTR)
            {
                perror("read()");
                close(src_fd);
                close(dst_fd);
                exit(EXIT_FAILURE);
            }
            if (read_bytes == 0)
            {
                close(src_fd);
                close(dst_fd);
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
                close(dst_fd);
                exit(EXIT_FAILURE);
            }
            left -= write_bytes;
            offset += write_bytes;
        } while (left > 0);
    }
    
    close(src_fd);
    close(dst_fd);
    
    exit(EXIT_SUCCESS);
}
