#include <stdio.h>
#include <stdlib.h>
#include <glob.h>

int main(int argc, char const *argv[])
{
    if (argc < 2)
    {
        fprintf(stderr, "Usage: %s <'pattern'>", argv[0]);
        exit(1);
    }
    
    glob_t dir_glob;
    int res;
    if ((res = glob(argv[1], 0, NULL, &dir_glob)) != 0)
    {
        printf("ERROR CODE: %d\n", res);
        exit(2);
    }
    printf("count is %zu\n", dir_glob.gl_pathc);
    for (int i = 0; i < dir_glob.gl_pathc; i++)
    {
        puts(dir_glob.gl_pathv[i]);
    }
    #if 0
    int i = 0;
    while (dir_glob.gl_pathv[i] != NULL)
    {
        puts(dir_glob.gl_pathv[i]);
        i++;
    }
    #endif
    globfree(&dir_glob);
    exit(0);
}
