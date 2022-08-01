#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include "relayer.h"

#define DATA1    "data1"
#define DATA2    "data2"
#define DATA3    "data3"
#define DATA4    "data4"


int main(int argc, char const *argv[])
{
    int fd1, fd2, fd3, fd4;
    int ret1, ret2;

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

    if ((fd3 = open(DATA3, O_RDWR | O_CREAT | O_TRUNC, 0666)) < 0)
    {
        perror("open fd1 fail");
        exit(EXIT_FAILURE);
    }

    if ((fd4 = open(DATA4, O_RDWR | O_CREAT | O_TRUNC | O_NONBLOCK, 0666)) < 0)
    {
        perror("open fd2 fail");
        exit(EXIT_FAILURE);
    }

    if ((ret1 = relayer_init(fd1, fd2)) < 0)
    {
        fprintf(stderr, "%s\n", relayer_err_str(ret1));
        exit(EXIT_FAILURE);
    }

    if ((ret2 = relayer_init(fd3, fd4)) < 0)
    {
        fprintf(stderr, "%s\n", relayer_err_str(ret2));
        exit(EXIT_FAILURE);
    }

    relayer_destroy(ret1);
    relayer_destroy(ret2);

    close(fd1);
    close(fd2);
    close(fd3);
    close(fd4);

    exit(EXIT_SUCCESS);
}