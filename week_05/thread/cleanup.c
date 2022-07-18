#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

static void clean(void *str)
{
    puts(str);
}

static void *thread1(void *arg)
{
    puts("thread1 is working");
    pthread_cleanup_push(clean, "1");
    pthread_cleanup_push(clean, "2");
    pthread_cleanup_push(clean, "3");
    puts("push complete");
    
    pthread_exit(NULL);
    
    pthread_cleanup_pop(1);
    pthread_cleanup_pop(0);
    pthread_cleanup_pop(0);
}

int main(int argc, char const *argv[])
{
    puts("begin");

    pthread_t tid;
    int ret;
    if ((ret = pthread_create(&tid, NULL, thread1, NULL)))
    {
        fprintf(stderr, "pthread_create(): %s\n", strerror(ret));
        exit(EXIT_FAILURE);
    }
    if ((ret = pthread_join(tid, NULL)))
    {
        fprintf(stderr, "pthread_join(): %s\n", strerror(ret));
        exit(EXIT_FAILURE);
    }
    puts("end");
    exit(EXIT_SUCCESS);
}
