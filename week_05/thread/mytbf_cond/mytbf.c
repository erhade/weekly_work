#include <errno.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <stdbool.h>

#include "mytbf.h"

typedef struct mytbf_st
{
    int cps;
    int burst;
    int token;
    int pos;
    pthread_mutex_t mut_token;
    pthread_cond_t cond_token;
}mytbf_st;

static mytbf_st *job[TBF_MAX];
static pthread_mutex_t mut_job = PTHREAD_MUTEX_INITIALIZER;
static pthread_once_t once = PTHREAD_ONCE_INIT;
static pthread_t pid; 

static int _find_pos_unlock()
{
    for (int i = 0; i < TBF_MAX; i++)
    {
        if (job[i] == NULL)
            return i;
    }
    return -1;
}

static void *alrm_thread(void *arg)
{
    while (true)
    {
        useconds_t sec = 1000000;
        pthread_mutex_lock(&mut_job);
        for (int i = 0; i < TBF_MAX; i++)
        {
            if (job[i] != NULL)
            {
                pthread_mutex_lock(&job[i]->mut_token);
                job[i]->token += job[i]->cps;
                if (job[i]->token > job[i]->burst)
                    job[i]->token = job[i]->burst;
                pthread_cond_broadcast(&job[i]->cond_token);
                pthread_mutex_unlock(&job[i]->mut_token);
            }
        }
        pthread_mutex_unlock(&mut_job);

        usleep(sec);
    }
    pthread_exit(NULL);
}

static void _alrm_unload_unlock(void)
{
    int ret;
    for (int i = 0; i < TBF_MAX; i++)
    {
        pthread_mutex_destroy(&job[i]->mut_token);
        pthread_cond_destroy(&job[i]->cond_token);
        free(job[i]);
        job[i] = NULL;
    }
    
    if ((ret = pthread_cancel(pid)))
    {
        fprintf(stderr, "pthread_cancel: %s\n", strerror(ret));
        exit(EXIT_FAILURE);
    }
    if ((ret = pthread_join(pid, NULL)))
    {
        fprintf(stderr, "pthread_cancel: %s\n", strerror(ret));
        exit(EXIT_FAILURE);
    }
}

static void _alrm_load(void)
{   
    int ret;
    if ((ret = pthread_create(&pid, NULL, alrm_thread, NULL)))
    {
        fprintf(stderr, "pthread_create: %s", strerror(ret));
        exit(EXIT_FAILURE);
    }
    atexit(_alrm_unload_unlock);
}

tbf_t *tbf_init(int cps, int burst)
{
    mytbf_st *tbf = malloc(sizeof(*tbf));
    if (tbf == NULL)
        return NULL;
    tbf->cps = cps;
    tbf->burst = burst;
    tbf->token = 0;
    pthread_mutex_init(&tbf->mut_token, NULL);
    pthread_cond_init(&tbf->cond_token, NULL);

    pthread_mutex_lock(&mut_job);
    int pos = _find_pos_unlock();
    if (pos == -1)
    {
        pthread_mutex_unlock(&mut_job);
        free(tbf);
        return NULL;
    }
    tbf->pos = pos;
    job[pos] = tbf;
    pthread_mutex_unlock(&mut_job);

    pthread_once(&once, _alrm_load);
    return tbf;
}

int tbf_fetch_token(tbf_t *tbf, int fet_num)
{
    if (fet_num <= 0 || tbf == NULL)
        return -EINVAL;
    mytbf_st *mytbf = tbf;

    pthread_mutex_lock(&mytbf->mut_token);
    while (mytbf->token <= 0)
    {
        pthread_cond_wait(&mytbf->cond_token, &mytbf->mut_token);
    }
    int actual = fet_num <= mytbf->token ? fet_num : mytbf->token;
    mytbf->token -= actual;
    pthread_mutex_unlock(&mytbf->mut_token);
    return actual;
}

int tbf_return_token(tbf_t *tbf, int ret_num)
{
    if (tbf == NULL || ret_num <= 0)
        return -EINVAL;
    mytbf_st *mytbf = tbf;
    pthread_mutex_lock(&mytbf->mut_token);
    mytbf->token += ret_num;
    if (mytbf->token > mytbf->burst)
        mytbf->token = mytbf->burst;
    pthread_cond_broadcast(&mytbf->cond_token);
    pthread_mutex_unlock(&mytbf->mut_token);
    return ret_num;
}

void tbf_destroy(tbf_t **tbf)
{
    mytbf_st *mytbf = *tbf;
    pthread_mutex_lock(&mut_job);
    job[mytbf->pos] = NULL;
    pthread_mutex_destroy(&mytbf->mut_token);
    pthread_cond_destroy(&mytbf->cond_token);
    free(mytbf);
    *tbf = NULL;
    // bool flag = true;
    // for (int i = 0; i < TBF_MAX; i++)
    // {
    //     if (job[i] != NULL)
    //         flag = false;
    // }
    // if (flag)
    //     _alrm_unload_unlock();
    pthread_mutex_unlock(&mut_job);
}