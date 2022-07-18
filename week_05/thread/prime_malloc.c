#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <pthread.h>
#include <string.h>

#define LEFT    30000000
#define RIGHT   40000000
#define T_NUM   4

typedef struct data_bundle_st
{
    int n;
}data_bundle_st;

static bool is_prime(int num)
{
    for (int i = 2; i <= (int)sqrt((num * 1.0)); i++)
    {
        if (num % i == 0)
            return false;
    }
    return true;
}

static void *thread(void *_data)
{
    data_bundle_st *data = _data;
    for (int i = LEFT; i <= RIGHT; i++)
    {
        if (i % T_NUM == data->n && is_prime(i))
        {
            printf("%d is prime\n", i);
        }
    }
    pthread_exit(_data);
}

int main(int argc, char const *argv[])
{
    int ret;
    pthread_t tid[T_NUM];
    data_bundle_st *data = NULL;
    void *tmp = NULL;
    for (int i = 0; i < T_NUM; i++)
    {
        data = malloc(sizeof(*data));
        data->n = i;
        if (data == NULL)
        {
            perror("malloc fail ");
            exit(EXIT_FAILURE);
        }
        if ((ret = pthread_create(tid+i, NULL, thread, data)))
        {
            fprintf(stderr, "create tid%d: %s\n", i, strerror(ret));
            exit(EXIT_FAILURE);
        }
    }

    for (int i = 0; i < T_NUM; i++)
    {
        if ((ret = pthread_join(tid[i], &tmp)))
        {
            fprintf(stderr, "join tid%d: %s\n", i, strerror(ret));
            exit(EXIT_FAILURE);
        }
        free(tmp);
    }

    exit(EXIT_SUCCESS);
}
