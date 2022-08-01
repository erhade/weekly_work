#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#define BUF_SIZE    1024

int main(int argc, char const *argv[])
{
    pid_t pid;
    int pipefd[2];
    char buf[BUF_SIZE];

    if (pipe(pipefd) < 0)
    {
        perror("pipe()");
        exit(EXIT_FAILURE);
    }

    pid = fork();
    if (pid < 0)
    {
        close(pipefd[0]);
        close(pipefd[1]);
        perror("fork()");
        exit(EXIT_FAILURE);
    }
    if (pid == 0)
    {
        int rd_num;
        close(pipefd[1]);
        rd_num = read(pipefd[0], buf, BUF_SIZE);
        write(1, buf, rd_num);
        close(pipefd[0]);
        exit(EXIT_SUCCESS);
    }
    
    close(pipefd[0]);
    write(pipefd[1], "hello", 6);
    write(pipefd[1], "world", 6);
    close(pipefd[1]);

    wait(NULL);

    exit(EXIT_SUCCESS);
}
