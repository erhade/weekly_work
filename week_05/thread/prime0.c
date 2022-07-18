#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <pthread.h>
#include <string.h>

#define LEFT    30000000
#define RIGHT   40000000
#define T_NUM   4

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
    int *num = _num;
    for (int i = LEFT; i <= RIGHT; i++)
    {
        if (i % T_NUM == (*num) && is_prime(i))
        {
            printf("%d is prime\n", i);
        }
    }
    pthread_exit(NULL);
}

int main(int argc, char const *argv[])
{
    int num[T_NUM], ret;
    pthread_t tid[T_NUM];
    for (int i = 0; i < T_NUM; i++)
    {
        num[i] = i;
        if ((ret = pthread_create(tid+i, NULL, thread, num+i)))
        {
            fprintf(stderr, "create tid%d: %s\n", i, strerror(ret));
            exit(EXIT_FAILURE);
        }
    }

    for (int i = 0; i < T_NUM; i++)
    {
        if ((ret = pthread_join(tid[i], NULL)))
        {
            fprintf(stderr, "join tid%d: %s\n", i, strerror(ret));
            exit(EXIT_FAILURE);
        }
    }

    exit(EXIT_SUCCESS);
}
