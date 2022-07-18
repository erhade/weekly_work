#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

static void *func(void *arg)
{
    puts("pthread is working");
    pthread_exit(NULL);
}

int main(int argc, char const *argv[])
{
    puts("begin");
    pthread_t tid;
    pthread_create(&tid, NULL, func, NULL);
    pthread_join(tid, NULL);
    puts("end");
    exit(EXIT_SUCCESS);
}
