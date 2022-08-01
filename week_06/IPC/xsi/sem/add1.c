#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <errno.h>

#define PROC_NUM     30
#define FILE_NAME   "/tmp/out"
#define BUF_SIZE    1024

static int sem_id;

static void P(void)
{
    struct sembuf buf;
    buf.sem_num = 0;
    buf.sem_op = -1;
    buf.sem_flg = 0;
    while (semop(sem_id, &buf, 1) < 0)
    {
        if (errno == EINTR || errno == EAGAIN)
            continue;
        perror("semop()");
        exit(EXIT_FAILURE);
    }
}

static void V(void)
{
    struct sembuf buf;
    buf.sem_num = 0;
    buf.sem_op = 1;
    buf.sem_flg = 0;
    while (semop(sem_id, &buf, 1) < 0)
    {
        if (errno == EAGAIN || errno == EINTR)
            continue;
        perror("semop()");
        exit(EXIT_FAILURE);
    }
}

static void add_proc(void)
{
    char buf_line[BUF_SIZE];
    FILE *file = fopen(FILE_NAME, "r+");
    if (file == NULL)
    {
        perror("fopen()");
        exit(EXIT_FAILURE);
    }

    P();
    fgets(buf_line, BUF_SIZE, file);
    fseek(file, 0, SEEK_SET);
    fprintf(file, "%d\n", atoi(buf_line) + 1);
    fflush(file);
    V();

    fclose(file);
}

int main(int argc, char const *argv[])
{
    pid_t pid;
    if ((sem_id = semget(IPC_PRIVATE, 1, 0666)) < 0)
    {
        perror("semget()");
        exit(EXIT_FAILURE);
    }

    semctl(sem_id, 0, SETVAL, 1);
    
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

    if (semctl(sem_id, 0, IPC_RMID) < 0)
    {
        perror("semctl()");
        exit(EXIT_FAILURE);
    }
    
    exit(EXIT_SUCCESS);
}
