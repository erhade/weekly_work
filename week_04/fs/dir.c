#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <dirent.h>

int main(int argc, char const *argv[])
{
    if (argc < 2)
    {
        fprintf(stderr, "Usage: %s <dirpath>\n", argv[0]);
        exit(1);
    }
    DIR *dir_path = NULL;
    if ((dir_path = opendir(argv[1])) == NULL)
    {
        perror("opendir() ");
        exit(2);
    }
    struct dirent *item = NULL;
    while ((item = readdir(dir_path)))
    {
        puts(item->d_name);
    }
    closedir(dir_path);
    exit(0);
}
