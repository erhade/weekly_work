#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <wait.h>

int main(int argc, char const *argv[])
{
    if (argc < 2)
    {
        fprintf(stderr, "Usage: %s <cmd>", argv[0]);
        exit(EXIT_FAILURE);
    }
    pid_t pid;
    fflush(NULL);
    pid = fork();
    if (pid < 0)
    {
        perror("fork()");
        exit(EXIT_FAILURE);
    }
    if (pid == 0)
    {
        execvp(argv[1], (char *const *)(argv + 1));
        perror("execvp()");
        exit(EXIT_FAILURE);
    }
    else
        wait(NULL);
    exit(EXIT_SUCCESS);
}
