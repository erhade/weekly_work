#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

#define MYSIGRT (SIGRTMIN+6)

static void signal_handler(int s)
{
    write(1, "!", 1);
}

int main(int argc, char const *argv[])
{
    sig_t signal_handler_save = signal(MYSIGRT, signal_handler);
    if (signal_handler_save == SIG_ERR)
    {
        perror("Unable bind MYSIGRT");
        exit(EXIT_FAILURE);
    }
    sigset_t old_set, unblock_set;
    useconds_t usec = 1000000;
    sigset_t set;
    sigemptyset(&set);
    sigaddset(&set, MYSIGRT);
    if (sigprocmask(SIG_UNBLOCK, &set, &old_set) < 0)
    {
        perror("sigprocmask()");
        exit(EXIT_FAILURE);
    }
    if (sigprocmask(SIG_BLOCK, &set, &unblock_set) < 0)
    {
        perror("sigprocmask()");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < 100; i++)
    {
        for (int j = 0; j < 5; j++)
        {
            write(1, "*", 1);
            usleep(usec);
        }
        write(1, "\n", 1);
        sigsuspend(&unblock_set);
    }
    if (sigprocmask(SIG_SETMASK, &old_set, NULL) < 0)
    {
        perror("sigprocmask()");
        exit(EXIT_FAILURE);
    }
    if (signal(MYSIGRT, signal_handler_save) == SIG_ERR)
    {
        perror("Unable to unbind MYSIGRT");
        exit(EXIT_FAILURE);
    }    
    
    exit(EXIT_SUCCESS);
}