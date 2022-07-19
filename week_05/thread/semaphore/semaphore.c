#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#include "semaphore.h"

typedef struct sem_st
{
    int size;
    int value;
    pthread_mutex_t mut;
    pthread_cond_t cond;
}sem_st;

sem_t *semaphore_init(int init_val)
{
    if (init_val <= 0)
        return NULL;
    sem_st *mysem = malloc(sizeof(*mysem));
    if (mysem == NULL)
        return NULL;
    mysem->value = init_val;
    mysem->size = init_val;
    pthread_mutex_init(&mysem->mut, NULL);
    pthread_cond_init(&mysem->cond, NULL);
    return mysem;
}

int semaphore_add(sem_t *sem, int add_val)
{
    if (sem == NULL)
        return -1;
    if (add_val <= 0)
        return -2;
    sem_st *mysem = sem;
    pthread_mutex_lock(&mysem->mut);
    while (mysem->value == mysem->size)
        pthread_cond_wait(&mysem->cond, &mysem->mut);
    mysem->value += add_val;
    if (mysem->value > mysem->size)
        mysem->value = mysem->size;
    pthread_cond_broadcast(&mysem->cond);
    pthread_mutex_unlock(&mysem->mut);
    return add_val;
}

int semaphore_sub(sem_t *sem, int sub_val)
{
    if (sem == NULL)
        return -1;
    if (sub_val <= 0)
        return -2;
    sem_st *mysem = sem;
    pthread_mutex_lock(&mysem->mut);
    while (mysem->value < sub_val)
        pthread_cond_wait(&mysem->cond, &mysem->mut);
    mysem->value -= sub_val;
    pthread_cond_broadcast(&mysem->cond);
    pthread_mutex_unlock(&mysem->mut);
    return sub_val;
}

int semaphore_destroy(sem_t *sem)
{
    if (sem == NULL)
        return -1;
    sem_st *mysem = sem;
    pthread_mutex_destroy(&mysem->mut);
    pthread_cond_destroy(&mysem->cond);
    free(sem);
    return 0;
}