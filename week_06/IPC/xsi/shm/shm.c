#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

#define SHM_SIZE    1024

int main(int argc, char const *argv[])
{
    int shm_id;
    int pid;

    if ((shm_id = shmget(IPC_PRIVATE, SHM_SIZE, 0666)) < 0)
    {
        perror("shmget()");
        exit(EXIT_FAILURE);
    }

    pid = fork();
    if (pid < 0)
    {
        perror("fork()");
        exit(EXIT_FAILURE);
    }
    if (pid == 0)
    {
        char *ptr = NULL;
        if ((ptr = shmat(shm_id, NULL, 0)) == (void *) -1)
        {
            perror("shmat()");
            exit(EXIT_FAILURE);
        }

        strcpy(ptr, "hello");
        shmdt(ptr);
        exit(EXIT_SUCCESS);
    }

    wait(NULL);
    char *ptr;
    if ((ptr = shmat(shm_id, NULL, 0)) == (void *) -1)
    {
        perror("shmat()");
        exit(EXIT_FAILURE);   
    }

    puts(ptr);
    shmdt(ptr);
    if (shmctl(shm_id, IPC_RMID, NULL) < 0)
    {
        perror("shmctl()");
        exit(EXIT_FAILURE);
    }

    exit(EXIT_SUCCESS);
}
