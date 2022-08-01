#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

#define PROC_NUM     30
#define FILE_NAME   "/tmp/out"
#define BUF_SIZE    1024

static void add_proc(void)
{
    char buf_line[BUF_SIZE];
    FILE *file = fopen(FILE_NAME, "r+");
    if (file == NULL)
    {
        perror("fopen()");
        exit(EXIT_FAILURE);
    }

    int fd = fileno(file);

    lockf(fd, F_LOCK, 0);
    fgets(buf_line, BUF_SIZE, file);
    fseek(file, 0, SEEK_SET);
    fprintf(file, "%d\n", atoi(buf_line) + 1);
    lockf(fd, F_ULOCK, 0);

    fclose(file);
}

int main(int argc, char const *argv[])
{
    pid_t pid;
    
    for (int i = 0; i < PROC_NUM; i++)
    {
        pid = fork();
        if (pid < 0)
        {
            perror("fork()");
            exit(EXIT_FAILURE);
        }
        else if (pid == 0)
        {
            add_proc();
            exit(EXIT_SUCCESS);
        }
    }

    for (int i = 0; i < PROC_NUM; i++)
        wait(NULL);
    
    exit(EXIT_SUCCESS);
}
