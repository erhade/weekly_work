#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

static off_t file_len(const char *file_name)
{
    struct stat file_stat;
    stat(file_name, &file_stat);
    return file_stat.st_size;
}

int main(int argc, char const *argv[])
{
    if (argc < 2)
    {
        fprintf(stderr, "Usage: %s <file_name>", argv[0]);
        exit(1);
    }
    printf("%ld\n", file_len(argv[1]));
    exit(0);
}
