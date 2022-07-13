#include <errno.h>
#include <signal.h>
#include <unistd.h>
#include <stdbool.h>
#include <sys/time.h>

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
static struct sigaction sa_save;

static int _find_pos()
{
    for (int i = 0; i < TBF_MAX; i++)
    {
        if (job[i] == NULL)
            return i;
    }
    return -1;
}

static void action_handler(int s, siginfo_t *infop, void *unused)
{
    if (infop->si_code != SI_KERNEL)
        return;
    for (int i = 0; i < TBF_MAX; i++)
    {
        if (job[i] != NULL)
        {
            job[i]->token += job[i]->cps;
            if (job[i]->token > job[i]->burst)
                job[i]->token = job[i]->burst;
        }
    }
    return;
}

static void _timer_unload(void)
{
    struct itimerval itv;
    itv.it_interval.tv_sec = 0;
    itv.it_interval.tv_usec = 0;
    itv.it_value = itv.it_interval;
    if (setitimer(ITIMER_REAL, &itv, NULL) < 0)
    {
        perror("setitimer() unload fail");
        exit(EXIT_FAILURE);
    }
    if (sigaction(SIGALRM, &sa_save, NULL) < 0)
    {
        perror("sigaction() unload fail");
        exit(EXIT_FAILURE);
    }
}

static void _timer_load(void)
{
    struct sigaction sa;
    sa.sa_sigaction = action_handler;
    sa.sa_flags = SA_SIGINFO;
    sigemptyset(&sa.sa_mask);
    if (sigaction(SIGALRM, &sa, &sa_save) < 0)
    {
        perror("sigsaction() load fail");
        exit(EXIT_FAILURE);
    }
    struct itimerval itv;
    itv.it_interval.tv_sec = 1;
    itv.it_interval.tv_usec = 0;
    itv.it_value = itv.it_interval; 
    if (setitimer(ITIMER_REAL, &itv, NULL) < 0)
    {
        perror("setitimer() load fail");
        exit(EXIT_FAILURE);
    }
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
        _timer_load();   
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
        _timer_unload();
}