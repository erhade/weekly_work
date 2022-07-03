#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <glob.h>

#define MAX_PATH   256

static size_t is_file(const char *path_name)
{
    struct stat file_stat;
    if (lstat(path_name, &file_stat) < 0)
    {
        perror("stat() ");
        exit(2);
    }
    if ((file_stat.st_mode & S_IFMT) == S_IFDIR)
        return -1;
    else
        return (file_stat.st_blocks / 2);
}

static int is_loop(const char *file_name)
{
    const char *file = strrchr(file_name, '/');
    if (strcmp(file + 1, ".") == 0 || strcmp(file + 1, "..") == 0)
        return 1;
    else
        return 0;
}

static size_t get_size(const char *path_name)
{
    size_t total_size = 0, file_size = 0;
    if ((file_size = is_file(path_name)) != -1)
    {
        total_size += file_size;
        return total_size;
    }
    else
    {
        char *next_path = malloc(MAX_PATH); 
        strncpy(next_path, path_name, MAX_PATH - 1);
        strncat(next_path, "/*", MAX_PATH - strlen(next_path) - 1);
        next_path[MAX_PATH-1] = '\0';
        glob_t dir;
        int ret = glob(next_path, 0, NULL, &dir);
        if (ret != 0 && ret != 3)
        {
            free(next_path);
            printf("(1) Error No is %d\n", ret);
            exit(3);
        }
        memset(next_path, 0, MAX_PATH);
        strncpy(next_path, path_name, MAX_PATH - 1);
        strncat(next_path, "/.*", MAX_PATH - strlen(next_path) - 1);
        ret = glob(next_path, GLOB_APPEND, NULL, &dir);
        if (ret != 0 && ret != 3)
        {
            free(next_path);
            printf("(2) Error No is %d\n", ret);
            exit(3);
        }
        free(next_path);
        for (int i = 0; i < dir.gl_pathc; i++)
        {
            if (is_loop(dir.gl_pathv[i]))
                continue;
            total_size += get_size(dir.gl_pathv[i]);
        }
        globfree(&dir);
        return total_size;
    }
}

int main(int argc, char const *argv[])
{
    if (argc < 2)
    {
        fprintf(stderr, "Usage: %s <pathname>\n", argv[0]);
        exit(1);
    }
    size_t size = get_size(argv[1]);
    printf("%zu\n", size);
    exit(0);
}
