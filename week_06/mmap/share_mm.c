#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define LEN    1024

static void mmap_clean(int s, void *arg)
{
    munmap(arg, LEN);
}

int main(int argc, char const *argv[])
{
    char *buf;
    pid_t pid;

    if ((buf = mmap(NULL, LEN, PROT_READ | PROT_WRITE, 
    MAP_SHARED | MAP_ANONYMOUS, -1, 0)) == MAP_FAILED)
    {
        perror("mmap()");
        exit(EXIT_FAILURE);
    }
    on_exit(mmap_clean, buf);

    fflush(NULL);

    pid = fork();
    if (pid < 0)
    {
        perror("fork()");
        exit(EXIT_FAILURE);
    }

    else if (pid == 0)
    {
        strncpy(buf, "hello", LEN-1);
        buf[LEN-1] = '\0';
        exit(EXIT_SUCCESS);   
    }

    wait(NULL);
    puts(buf);

    exit(EXIT_SUCCESS);
}
