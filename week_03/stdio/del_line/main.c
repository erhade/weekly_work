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
        fprintf(stderr, "Usage: %s <file> <del_line>", argv[0]);
        exit(1);
    }
    int rd, wr;
    rd = open(argv[1], O_RDONLY);
    if (rd < 0)
    {
        perror("open() ");
        exit(2);
    }
    wr = open(argv[1], O_RDWR);
    if (wr < 0)
    {
        close(rd);
        perror("open() ");
        exit(2);
    }
    int line_no = atoi(argv[2]);
    char buf[BUF_LEN];
    int rd_offset = 0, wr_offset = 0, offset = 0, line = 1;
    int ret;
    while ((ret = read(rd, buf, 1)))
    {
        if (line < line_no)
        {
            wr_offset++;
        }
        if (line < line_no + 1)
        {
            rd_offset++;
        }
        if (buf[0] == '\n')
        {
            line++;  
        }
        offset++;
    }
    if (ret < 0)
    {
        close(rd);
        close(wr);
        perror("read() ");
        exit(3);
    }
    if (line < line_no)
    {
        close(rd);
        close(wr);
        fprintf(stderr, "del_line error\n");
        exit(4);
    }
    if (line == line_no)
    {
        ftruncate(wr, wr_offset);
        close(rd);
        close(wr);
        exit(0);
    }
    if (lseek(wr, wr_offset, SEEK_SET) < 0)
    {
        perror("seek() ");
        exit(5);
    }
    if (lseek(rd, rd_offset, SEEK_SET) < 0)
    {
        perror("seek() ");
        exit(5);
    }
    int rd_num, wr_num, pos;
    while (1)
    {
        rd_num = read(rd, buf, BUF_LEN);
        if (rd_num < 0)
        {
            perror("read() ");
            break;
        }
        if (rd_num == 0)
            break;
        pos = 0;
        while (rd_num > 0)
        {
            wr_num = write(wr, buf + pos, rd_num);
            if (wr_num < 0)
            {
                perror("write() ");
                close(rd);
                close(wr);
                exit(6);
            }
            rd_num -= wr_num;
            pos += wr_num;
        }
    }
    ftruncate(wr, offset - (rd_offset - wr_offset));
    close(rd);
    close(wr);
    exit(0);
}
