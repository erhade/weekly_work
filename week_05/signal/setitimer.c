#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/time.h>

static sig_atomic_t volatile num = 0;
static sig_t alarm_handler_save = NULL;
static struct itimerval itv;

static void alarm_handler(int s)
{
    printf("alarm [%d]\n", num++);
}

static void alarm_load()
{
    int ret;
    if ((alarm_handler_save = signal(SIGALRM, alarm_handler)) == SIG_ERR)
    {
        perror("Unable to bind SIG_ALRM");
        exit(EXIT_FAILURE);
    }
    itv.it_interval.tv_sec = 1;
    itv.it_interval.tv_usec = 0;
    itv.it_value = itv.it_interval; 
    ret = setitimer(ITIMER_REAL, &itv, NULL);
    if (ret < 0)
    {
        perror("setitimer() load");
        exit(EXIT_FAILURE);
    }
}

static void alarm_unload()
{
    int ret;
    itv.it_interval.tv_sec = 0;
    itv.it_interval.tv_usec = 0;
    itv.it_value.tv_sec = 0;
    itv.it_value.tv_usec = 0;
    ret = setitimer(ITIMER_REAL, &itv, NULL);
    if (ret < 0)
    {
        perror("setitimer() unload");
        exit(EXIT_FAILURE);
    }
    if (signal(SIGALRM, alarm_handler_save) == SIG_ERR)
    {
        perror("Unable to bind SIGALRM");
        exit(EXIT_FAILURE);
    }
}

int main(int argc, char const *argv[])
{
    alarm_load();
    while (num <= 10)
    {
        num += 2;
        pause();
    }
    alarm_unload();
    exit(EXIT_SUCCESS);
}
