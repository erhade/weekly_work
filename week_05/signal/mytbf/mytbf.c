#include <errno.h>
#include <signal.h>
#include <unistd.h>
#include <stdbool.h>

#include "mytbf.h"

typedef struct mytbf_st
{
    int cps;
    int burst;
    sig_atomic_t token;
    int pos;
}mytbf_st;

static mytbf_st *job[TBF_MAX];
static bool inited = true;
sig_t alrm_handler_save = NULL;

static int _find_pos()
{
    for (int i = 0; i < TBF_MAX; i++)
    {
        if (job[i] == NULL)
            return i;
    }
    return -1;
}

static void alrm_handler(int s)
{
    for (int i = 0; i < TBF_MAX; i++)
    {
        if (job[i] != NULL)
        {
            job[i]->token += job[i]->cps;
            if (job[i]->token > job[i]->burst)
                job[i]->token = job[i]->burst;
        }
    }
    
    alarm(1);
}

static void _alrm_unload(void)
{
    alarm(0);
    signal(SIGALRM, alrm_handler_save);
}

static void _alrm_load(void)
{   
    alrm_handler_save = signal(SIGALRM, alrm_handler);
    alarm(1);
}

tbf_t *tbf_init(int cps, int burst)
{
    int pos = _find_pos();
    mytbf_st *tbf = malloc(sizeof(*tbf));
    if (tbf == NULL || pos == -1)
        return NULL;
    tbf->cps = cps;
    tbf->burst = burst;
    tbf->token = 0;
    tbf->pos = pos;
    job[pos] = tbf;
    if (inited)
    {
        _alrm_load();   
        inited = false;
    }
    return tbf;
}

int tbf_fetch_token(tbf_t *tbf, int fet_num)
{
    if (fet_num <= 0 || tbf == NULL)
        return -EINVAL;
    mytbf_st *mytbf = tbf;
    while (mytbf->token <= 0)
        pause();
    int actual = fet_num <= mytbf->token ? fet_num : mytbf->token;
    mytbf->token -= actual;
    return actual;
}

int tbf_return_token(tbf_t *tbf, int ret_num)
{
    if (tbf == NULL || ret_num <= 0)
        return -EINVAL;
    mytbf_st *mytbf = tbf;
    mytbf->token += ret_num;
    if (mytbf->token > mytbf->burst)
        mytbf->token = mytbf->burst;
    return ret_num;
}

void tbf_destroy(tbf_t **tbf)
{
    mytbf_st *mytbf = *tbf;
    job[mytbf->pos] = NULL;
    free(mytbf);
    *tbf = NULL;
    bool flag = true;
    for (int i = 0; i < TBF_MAX; i++)
    {
        if (job[i] != NULL)
            flag = false;
    }
    if (flag)
        _alrm_unload();
}