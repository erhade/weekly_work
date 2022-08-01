#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(int argc, char const *argv[])
{
    pid_t pid;
    int pipefd[2];

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
        close(pipefd[1]);
        dup2(pipefd[0], 0);
        close(pipefd[0]);
        int fd = open("/dev/null", O_RDWR);
        dup2(fd, 1);
        dup2(fd, 2);
        close(fd);
        
        execl("/usr/bin/mpg123", "mpg123", "-", NULL);
        perror("execl()");
        exit(EXIT_FAILURE);
    }
    
    close(pipefd[0]);
    // 父进程收网络数据 写入到管道里 write();
    close(pipefd[1]);
    wait(NULL);
    exit(EXIT_SUCCESS);
}
