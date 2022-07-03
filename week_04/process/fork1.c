#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

int main(int argc, char const *argv[])
{
    printf("[%d] begin\n", getpid());
    fflush(NULL); /* !!! */
    pid_t pid = fork();
    if (pid < 0)
    {
        perror("fork()");
        exit(EXIT_FAILURE);
    }
    if (pid == 0)
    {
        printf("[%d] end\n", getpid());
    }
    else
    {
        printf("[%d] end\n", getpid());
    }
    exit(EXIT_SUCCESS);
}
