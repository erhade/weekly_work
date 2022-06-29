#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

int main(int argc, char const *argv[])
{
    if (argc < 2)
    {
        fprintf(stderr, "Usage: %s <filepath>\n", argv[0]);
        exit(1);
    }
    struct stat file_stat;
    if (stat(argv[1], &file_stat) < 0)
    {
        perror("stat() ");
        exit(1);
    }
    printf("file type is ");
    switch (file_stat.st_mode & S_IFMT)
    {
    case S_IFSOCK:
        printf("socket\n");
        break;
    case S_IFLNK:
        printf("symbolic link\n");
        break;
    case S_IFREG:
        printf("regular file\n");
        break;
    case S_IFBLK:
        printf("block device\n");
        break;
    case S_IFCHR:
        printf("character device\n");
        break;
    case S_IFDIR:
        printf("directory\n");
        break;
    case S_IFIFO:
        printf("FIFO/pipe\n");
        break;
    default:
        printf("error\n");
        break;
    }

    exit(0);
}
