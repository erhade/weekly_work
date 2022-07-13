#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <stdbool.h>
#include <errno.h>
#include <sys/time.h>

#include "anytimer.h"

/* 错误码和错误处理的strerror最好自己去定义 内置errorno不好用 */

enum
{
    RUNNING = 1,
    OVER,
    CANCEL,
    STOP,
};

typedef struct anytimer_st
{
    sig_atomic_t sec;
    sig_atomic_t time_remain;
    JOB myjob;
    char *msg;
    sig_atomic_t status;
    bool is_repeat;
}anytimer_st;

static anytimer_st *jobs[JOB_MAX];
static bool inited = false;
static bool flag = true;
static struct sigaction sa_save;

static int _get_pos(void)
{
    for (int i = 0; i < JOB_MAX; i++)
    {
        if (jobs[i] == NULL)
            return i;
    }       
    return -1;
}

void anytimer_exit(void)
{
    inited = false;
    if (flag)
    {
        for (int i = 0; i < JOB_MAX; i++)
        {
            free(jobs[i]);
            jobs[i] = NULL;
        }
        flag = true;
    }
    
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

static void alrm_handler(int s, siginfo_t *infop, void *unused)
{
    if (infop->si_code != SI_KERNEL)
        return;
    for (int i = 0; i < JOB_MAX; i++)
    {
        if (jobs[i] != NULL && jobs[i]->status == RUNNING)
        {
            jobs[i]->sec--;
            if (jobs[i]->sec <= 0)
            {
                if (jobs[i]->is_repeat == false)
                {
                    jobs[i]->myjob(jobs[i]->msg);
                    jobs[i]->status = OVER;
                }
                else
                {
                    jobs[i]->myjob(jobs[i]->msg);
                    jobs[i]->sec = jobs[i]->time_remain;
                }
            }
        }
    }
    for (int i = 0; i < JOB_MAX; i++)
    {
        if (jobs[i] != NULL)
            return;
    }
    flag = false;
    anytimer_exit();
    return;
}

int anytimer_init(int sec, JOB func, char *msg, bool is_repeat)
{
    int pos;
    if (sec <= 0 || func == NULL || msg == NULL)
        return -EINVAL;
    if ((pos = _get_pos()) < 0)
        return -ENOSPC;
    anytimer_st *my_timer = malloc(sizeof(*my_timer));
    if (my_timer == NULL)
        return -ENOMEM;
    my_timer->sec = sec;
    my_timer->time_remain = sec;
    my_timer->is_repeat = is_repeat;
    my_timer->myjob = func;
    my_timer->msg = msg;
    my_timer->status = RUNNING;
    jobs[pos] = my_timer;
    if (!inited)
    {
        struct sigaction sa;
        sa.sa_sigaction = alrm_handler;
        sa.sa_flags = SA_SIGINFO;
        sigemptyset(&sa.sa_mask);
        if (sigaction(SIGALRM, &sa, &sa_save) < 0)
        {
            perror("sigaction() load fail");
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
        inited = true;
        atexit(anytimer_exit);
    }
    return pos;
}

int anytimer_cancel(int job_id)
{
    if (job_id < 0 || job_id >= JOB_MAX)
        return -EINVAL;
    if (jobs[job_id] == NULL)
        return -EBUSY;
    if (jobs[job_id]->status == RUNNING)
    {
        jobs[job_id]->status = CANCEL;
        return 0;
    }
    return -ECANCELED;
}

int anytimer_wait(int job_id)
{
    if (job_id < 0 || job_id >= JOB_MAX)
        return -EINVAL;
    if (jobs[job_id] == NULL)
        return -EBUSY;
    while (jobs[job_id]->status != OVER && jobs[job_id]->status != CANCEL)
        pause();

    free(jobs[job_id]);
    jobs[job_id] = NULL;
    return 0;
}

int anytimer_stop(int job_id)
{
    if (job_id < 0 || job_id >= JOB_MAX)
        return -EINVAL;
    if (jobs[job_id] == NULL)
        return -EBUSY;
    if (jobs[job_id]->status == RUNNING)
    {
        jobs[job_id]->status = STOP;
        return 0;
    }
    return -ESTALE;
}

int anytimer_resume(int job_id)
{
    if (job_id < 0 || job_id >= JOB_MAX)
        return -EINVAL;
    if (jobs[job_id] == NULL)
        return -EBUSY;
    if (jobs[job_id]->status == STOP)
    {
        jobs[job_id]->status = RUNNING;
        return 0;
    }
    return -ERESTART;
}