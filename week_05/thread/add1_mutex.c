#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>

#define PTH_NUM     30
#define FILE_NAME   "/tmp/out"
#define BUF_SIZE    1024

pthread_mutex_t mut = PTHREAD_MUTEX_INITIALIZER;

static void *add_thread(void *arg)
{
    char buf_line[BUF_SIZE];
    FILE *file = fopen(FILE_NAME, "r+");
    if (file == NULL)
    {
        perror("fopen()");
        exit(EXIT_FAILURE);
    }

    pthread_mutex_lock(&mut);
    fgets(buf_line, BUF_SIZE, file);
    fseek(file, 0, SEEK_SET);
    fprintf(file, "%d\n", atoi(buf_line) + 1);
    fclose(file);
    pthread_mutex_unlock(&mut);

    pthread_exit(NULL);
}

int main(int argc, char const *argv[])
{
    int ret;
    pthread_t thrds[PTH_NUM];
    for (int i = 0; i < PTH_NUM; i++)
    {
        if ((ret = pthread_create(thrds+i, NULL, add_thread, NULL)))
        {
            fprintf(stderr, "pthread_create(): %s\n", strerror(ret));
            exit(EXIT_FAILURE);
        }
    }

    for (int i = 0; i < PTH_NUM; i++)
    {
        if ((ret = pthread_join(thrds[i], NULL)))
        {
            fprintf(stderr, "pthread_join(): %s\n", strerror(ret));
            exit(EXIT_FAILURE);
        }
    }
    
    pthread_mutex_destroy(&mut);
    
    exit(EXIT_SUCCESS);
}
