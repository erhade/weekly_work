#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#define BUF_LEN    1024

int main(int argc, char const *argv[])
{
    if (argc < 3)
    {
        fprintf(stderr, "Usage: %s <src_dir> <dst_dir>\n", argv[0]);
        exit(1);
    }
    int src, dst;
    src = open(argv[1], O_RDONLY);
    if (src < 0)
    {
        perror("open() ");
        exit(2);
    }
    dst = open(argv[2], O_WRONLY | O_TRUNC | O_CREAT, 0666);
    if (dst < 0)
    {
        close(src);
        perror("open() ");
        exit(2);
    }
    char buf[BUF_LEN];
    int rd_len, wr_len;
    int pos;
    while (1)
    {
        rd_len = read(src, buf, BUF_LEN);
        if (rd_len < 0)
        {
            perror("read() ");
            break;
        }
        if (rd_len == 0)
            break;
        pos = 0;
        while (rd_len > 0)
        {
            wr_len = write(dst, buf+pos, rd_len);
            if (wr_len < 0)
            {
                perror("write() ");
                close(src);
                close(dst);
                exit(3);
            }
            rd_len -= wr_len;
            pos += wr_len;
        }
    }
    close(src);
    close(dst);
    exit(0);
}
