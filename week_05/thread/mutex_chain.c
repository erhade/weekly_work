#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

#define T_NUM    4

static pthread_mutex_t mutex[T_NUM];

static int next(int n)
{
    if (n+1 == T_NUM)
        return 0;
    return (n+1);
}

static void *my_thread(void *_num)
{
    int num = *(int *)_num;
    int val = num + 'a';
    for (int i = 0; i < 10; i++)
    {
        pthread_mutex_lock(mutex+num);
        write(1, &val, 1);
        pthread_mutex_unlock(mutex+next(num));
    }
    pthread_exit(NULL);
}

int main(int argc, char const *argv[])
{
    pthread_t T_ARR[T_NUM];
    int nums[T_NUM];
    int ret;
    for (int i = 0; i < T_NUM; i++)
    {
        pthread_mutex_init(mutex+i, NULL);
        pthread_mutex_lock(mutex+i);
        nums[i] = i;
        if ((ret = pthread_create(T_ARR+i, NULL, my_thread, nums+i)))
        {
            fprintf(stderr, "pthread_create: %s\n", strerror(ret));
            exit(EXIT_FAILURE);
        }
    }

    pthread_mutex_unlock(mutex + 0);
    
    for (int i = 0; i < T_NUM; i++)
    {
        if ((ret = pthread_join(T_ARR[i], NULL)))
        {
            fprintf(stderr, "pthread_join: %s\n", strerror(ret));
            exit(EXIT_FAILURE);
        }
        pthread_mutex_destroy(mutex+i);
    }

    exit(EXIT_SUCCESS);
}
