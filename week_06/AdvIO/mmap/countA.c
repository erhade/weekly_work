#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

static void close_clean(int s, void *arg)
{
    int fd = *(int *)arg;
    close(fd);
}

int main(int argc, char const *argv[])
{
    if (argc < 2)
    {
        fprintf(stderr, "Usage: %s <file_name>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int fd = open(argv[1], O_RDONLY);
    if (fd < 0)
    {
        perror("open()");
        exit(EXIT_FAILURE);
    }
    on_exit(close_clean, &fd);

    struct stat statres;
    if (fstat(fd, &statres) < 0)
    {
        perror("fstat()");
        exit(EXIT_FAILURE);
    }

    char *buf;
    if ((buf = mmap(NULL, statres.st_size, PROT_READ, MAP_PRIVATE, fd, 0)) == MAP_FAILED)
    {
        perror("mmap()");
        exit(EXIT_FAILURE);
    }

    int cnt = 0;
    for (int i = 0; i < statres.st_size; i++)
    {
        if (buf[i] == 'A' || buf[i] == 'a')
            cnt++;
    }
    printf("%d\n", cnt);

    if (munmap(buf, statres.st_size) < 0)
    {
        perror("munmap()");
        exit(EXIT_FAILURE);
    }

    exit(EXIT_SUCCESS);
}
