#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char const *argv[])
{
    if (argc < 2)
    {
        fprintf(stderr, "Usage: %s <filepath>\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    int fd = open(argv[1], O_WRONLY | O_CREAT | O_TRUNC, 0666);
    if (fd < 0)
    {
        perror("open() ");
        exit(EXIT_FAILURE);
    }
    lseek(fd, 5LL*1024*1024LL*1024LL - 1LL, SEEK_SET);
    write(fd, "", 1);
    close(fd);
    exit(EXIT_SUCCESS);
}
