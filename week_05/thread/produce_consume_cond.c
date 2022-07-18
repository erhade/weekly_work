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

pthread_mutex_t mut = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond_p = PTHREAD_COND_INITIALIZER;
pthread_cond_t cond_c = PTHREAD_COND_INITIALIZER;
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

static void *thread_func(void *arg)
{
    int no = *(int *)arg;
    
    while (true)
    {
        pthread_mutex_lock(&mut);
        while (num == 0)
            pthread_cond_wait(&cond_p, &mut);
        if (num == -1)
        {
            pthread_mutex_unlock(&mut);
            pthread_exit(NULL);
        }
        if (is_prime(num))
            printf("[%d] %d is a prime\n", no, num);
        num = 0;
        pthread_cond_signal(&cond_c);
        pthread_mutex_unlock(&mut);
    }
}

int main(int argc, char const *argv[])
{
    int ret;
    pthread_t threads[T_NUM];
    int thread_nums[T_NUM];

    for (int i = 0; i < T_NUM; i++)
    {
        thread_nums[i] = i;
        if ((ret = pthread_create(threads+i, NULL, thread_func, thread_nums+i)))
        {
            fprintf(stderr, "pthread_create: %s\n", strerror(ret));
            exit(EXIT_FAILURE);
        }
    }

    for (int i = LEFT; i <= RIGHT+1; i++)
    {
        pthread_mutex_lock(&mut);
        while (num != 0)
            pthread_cond_wait(&cond_c, &mut);
        if (i == RIGHT+1)
        {
            num = -1;
            pthread_cond_broadcast(&cond_p);
        }    
        else
        {
            num = i;
            pthread_cond_signal(&cond_p);
        }
        pthread_mutex_unlock(&mut);
    }

    for (int i = 0; i < T_NUM; i++)
    {
        if ((ret = pthread_join(threads[i], NULL)))
        {
            fprintf(stderr, "pthread_join: %s\n", strerror(ret));
            exit(EXIT_FAILURE);
        }
    }

    pthread_mutex_destroy(&mut);
    pthread_cond_destroy(&cond_c);
    pthread_cond_destroy(&cond_p);
    
    exit(EXIT_SUCCESS);
}
