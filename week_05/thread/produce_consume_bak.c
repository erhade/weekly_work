#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>

#define LEFT    30000000
#define RIGHT   30002000
#define T_NUM    4

pthread_mutex_t produce = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t consume = PTHREAD_MUTEX_INITIALIZER;
static int num = 0;

static bool is_prime(int num)
{
    for (int i = 2; i <= (int)sqrt(num * 1.0); i++)
    {
        if (num % i == 0)
            return false;
    }
    return true;
}

static void thread_clean(void *arg)
{
    pthread_mutex_unlock(&consume);
}

static void *thread_func(void *arg)
{
    int no = *(int *)arg;
    pthread_cleanup_push(thread_clean, NULL);
    pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);
    while (true)
    {
        pthread_mutex_lock(&consume);
        if (is_prime(num))
            printf("[%d] %d is a prime\n", no, num);
        pthread_mutex_unlock(&produce);
    }
    pthread_cleanup_pop(1);
}

int main(int argc, char const *argv[])
{
    int ret;
    pthread_t threads[T_NUM];
    int thread_nums[T_NUM];
    pthread_mutex_lock(&consume);
    for (int i = 0; i < T_NUM; i++)
    {
        thread_nums[i] = i;
        if ((ret = pthread_create(threads+i, NULL, thread_func, thread_nums+i)))
        {
            fprintf(stderr, "pthread_create: %s\n", strerror(ret));
            exit(EXIT_FAILURE);
        }
    }
    for (int i = LEFT; i <= RIGHT; i++)
    {
        pthread_mutex_lock(&produce);
        num = i;
        pthread_mutex_unlock(&consume);
    }
    pthread_mutex_lock(&produce);
    pthread_mutex_unlock(&produce);

    for (int i = 0; i < T_NUM; i++)
    {
        pthread_cancel(threads[i]);
    }

    pthread_mutex_destroy(&produce);
    pthread_mutex_destroy(&consume);

    for (int i = 0; i < T_NUM; i++)
    {
        if ((ret = pthread_join(threads[i], NULL)))
        {
            fprintf(stderr, "pthread_join: %s\n", strerror(ret));
            exit(EXIT_FAILURE);
        }
    }
    
    exit(EXIT_SUCCESS);
}
