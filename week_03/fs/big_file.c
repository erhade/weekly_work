#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char const *argv[])
{
    int fd = open("bigfile", O_WRONLY | O_CREAT | O_TRUNC, 0666);
    if (fd < 0)
    {
        perror("open() ");
        exit(1);
    }
    lseek(fd, 5LL*1024LL*1024LL*1024LL - 1LL, SEEK_SET);
    write(fd, "", 1);
    close(fd);
    exit(0);
}
