#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <wait.h>

int main(int argc, char const *argv[])
{
    puts("Begin");
    fflush(NULL);
    pid_t pid = fork();
    if (pid < 0)
    {
        perror("fork()");
        exit(EXIT_FAILURE);
    }
    if (pid == 0)
    {
        execl("/bin/date", "date", "+%s", NULL);
        perror("execl()");
        exit(EXIT_FAILURE);
    }
    wait(NULL);
    puts("end");
    exit(EXIT_SUCCESS);
}
