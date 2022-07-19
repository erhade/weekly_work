#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <pthread.h>
#include <stdbool.h>

#include "semaphore.h"

#define LEFT    30000000
#define RIGHT   30002000
#define T_NUM   4

sem_t *sem = NULL;

static bool is_prime(int num)
{
    for (int i = 2; i <= (int)sqrt((num * 1.0)); i++)
    {
        if (num % i == 0)
            return false;
    }
    return true;
}

static void *thread(void *_num)
{
    int num = *(int *)_num;
    if (is_prime(num))
    {
        printf("%d is prime\n", num);
    }
    
    semaphore_add(sem, 1);
    pthread_exit(NULL);
}

int main(int argc, char const *argv[])
{
    int num[RIGHT-LEFT+1], ret;
    pthread_t tid[RIGHT-LEFT+1];
    sem = semaphore_init(T_NUM);

    for (int i = LEFT; i <= RIGHT; i++)
    {
        semaphore_sub(sem, 1);
        num[i-LEFT] = i;
        if ((ret = pthread_create(tid+i-LEFT, NULL, thread, num+i-LEFT)))
        {
            fprintf(stderr, "create tid%d: %s\n", i, strerror(ret));
            exit(EXIT_FAILURE);
        }
    }

    for (int i = LEFT; i <= RIGHT; i++)
    {
        if ((ret = pthread_join(tid[i-LEFT], NULL)))
        {
            fprintf(stderr, "join tid%d: %s\n", i, strerror(ret));
            exit(EXIT_FAILURE);
        }
    }

    semaphore_destroy(sem);
    exit(EXIT_SUCCESS);
}
