#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <wait.h>
#include <stdbool.h>
#include <glob.h>

static void prompt(void)
{
    printf("mysh 0.1 $ ");
}

static bool parse(char *line_buf, glob_t *my_glob)
{
    char *delim = " \t\n";
    char *save = NULL;
    char *tmp = NULL;
    int i = false;
    int ret;
    while ((tmp = strtok_r(line_buf, delim, &save)) != NULL)
    {
        if (*tmp == '\0')
            continue;
        ret = glob(tmp, GLOB_NOCHECK | GLOB_APPEND * i, NULL, my_glob);
        if (ret != 0)
        {
            fprintf(stderr, "Error No is %d\n", ret);
            exit(EXIT_FAILURE);
        }
        i = true;
        line_buf = NULL;
    }
    return i;
}

int main(int argc, char const *argv[])
{
    char *line_buf;
    size_t line_num;
    glob_t my_glob;
    pid_t pid;
    while (true)
    {
        line_buf = NULL;
        line_num = 0;
        prompt();
        if (getline(&line_buf, &line_num, stdin) < 0)
        {
            perror("getline()");
            exit(EXIT_FAILURE);
        }
        if (parse(line_buf, &my_glob) == false)
        {
            free(line_buf);
            continue;
        }
        free(line_buf);
        fflush(NULL);
        pid = fork();
        if (pid < 0)
        {
            perror("fork()");
            globfree(&my_glob);
            exit(EXIT_FAILURE);
        }
        else if (pid == 0)
        {
            execvp(my_glob.gl_pathv[0], my_glob.gl_pathv);
            perror("execvp()");
            globfree(&my_glob);
            exit(EXIT_FAILURE);
        }
        else
        {
            wait(NULL);
            globfree(&my_glob);
        }
    }
    
    exit(EXIT_SUCCESS);
}
