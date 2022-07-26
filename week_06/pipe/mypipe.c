#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <pthread.h>

#include "mypipe.h"

typedef struct mypipe_st
{
    int head;
    int tail;
    char data[PIPE_SIZE];
    int data_size;
    int count_rd;
    int count_wr;
    pthread_mutex_t mut;
    pthread_cond_t cond;
}mypipe_st;

mypipe_t *mypipe_init(void)
{
    mypipe_st *pipe= malloc(sizeof(*pipe));
    if (pipe == NULL)
        return NULL;
    pipe->head = 0;
    pipe->tail = 0;
    memset(pipe->data, 0, PIPE_SIZE);
    pipe->data_size = 0;
    pipe->count_rd = 0;
    pipe->count_wr = 0;
    pthread_mutex_init(&pipe->mut, NULL);
    pthread_cond_init(&pipe->cond, NULL);
    return pipe;
}

int mypipe_register(mypipe_t *pipe, int flags)
{
    if (pipe == NULL)
        return -1;
    if (flags == 0)
        return -2;
    mypipe_st *mypipe = pipe;
    pthread_mutex_lock(&mypipe->mut);

    if (flags & PIPE_READ)
        mypipe->count_rd++;
    if (flags & PIPE_WRITE)
        mypipe->count_wr++;
    
    pthread_cond_broadcast(&mypipe->cond);

    while (mypipe->count_rd <= 0 || mypipe->count_wr <= 0)
        pthread_cond_wait(&mypipe->cond, &mypipe->mut);

    pthread_mutex_unlock(&mypipe->mut);
    return 0;
}

int mypipe_unregister(mypipe_t *pipe, int flags)
{
    if (pipe == NULL)
        return -1;
    if (flags == 0)
        return -2;
    mypipe_st *mypipe = pipe;
    pthread_mutex_lock(&mypipe->mut);

    if (flags & PIPE_READ)
        mypipe->count_rd--;
    if (flags & PIPE_WRITE)
        mypipe->count_wr--;

    pthread_cond_broadcast(&mypipe->cond);

    pthread_mutex_unlock(&mypipe->mut);
    return 0;
}

int mypipe_read(mypipe_t *pipe, char *buf, int len)
{
    if (pipe == NULL)
        return -1;
    if (buf == NULL)
        return -2;
    if (len <= 0 || len > PIPE_SIZE)
        return -3;
    mypipe_st *mypipe = pipe;
    pthread_mutex_lock(&mypipe->mut);

    while (mypipe->data_size <= 0 && mypipe->count_wr > 0)
        pthread_cond_wait(&mypipe->cond, &mypipe->mut);
    
    if (mypipe->data_size <= 0 && mypipe->count_wr <= 0)
    {
        pthread_mutex_unlock(&mypipe->mut);
        return 0;
    }

    int rd_len = mypipe->data_size <= len ? mypipe->data_size : len;
    strncpy(buf, mypipe->data+mypipe->head, rd_len);
    mypipe->data_size -= rd_len;
    mypipe->head = (mypipe->head + rd_len) % PIPE_SIZE;


    pthread_mutex_unlock(&mypipe->mut);
    return rd_len;
}

int mypipe_write(mypipe_t *pipe, const char *buf, int len)
{
    if (pipe == NULL)
        return -1;
    if (buf == NULL)
        return -2;
    if (len <= 0 || len > PIPE_SIZE)
        return -3;
    mypipe_st *mypipe = pipe;
    pthread_mutex_lock(&mypipe->mut);

    while (mypipe->data_size >= PIPE_SIZE && mypipe->count_rd > 0)
        pthread_cond_wait(&mypipe->cond, &mypipe->mut);
    
    if (mypipe->data_size >= PIPE_SIZE && mypipe->count_rd <= 0)
    {
        pthread_mutex_unlock(&mypipe->mut);
        return 0;
    }

    int wr_len = PIPE_SIZE-mypipe->data_size <= len ? PIPE_SIZE-mypipe->data_size : len;
    strncpy(mypipe->data+mypipe->tail, buf, wr_len);
    mypipe->data_size += wr_len;
    mypipe->tail = (mypipe->tail + wr_len) % PIPE_SIZE;


    pthread_mutex_unlock(&mypipe->mut);
    return wr_len;
}

int mypipe_destroy(mypipe_t *pipe)
{
    if (pipe == NULL)
        return -1;
    mypipe_st *mypipe = pipe;
    pthread_mutex_destroy(&mypipe->mut);
    pthread_cond_destroy(&mypipe->cond);
    free(mypipe);
    return 0;
}