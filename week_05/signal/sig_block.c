#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

static void signal_handler(int s)
{
    write(1, "!", 1);
}

int main(int argc, char const *argv[])
{
    sig_t signal_handler_save = signal(SIGINT, signal_handler);
    if (signal_handler_save == SIG_ERR)
    {
        perror("Unable bind SIGINT");
        exit(EXIT_FAILURE);
    }
    sigset_t old_set;
    useconds_t usec = 1000000;
    sigset_t set;
    sigemptyset(&set);
    sigaddset(&set, SIGINT);
    if (sigprocmask(SIG_UNBLOCK, NULL, &old_set) < 0)
    {
        perror("sigprocmask()");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < 100; i++)
    {
        if (sigprocmask(SIG_BLOCK, &set, NULL) < 0)
        {
            perror("sigprocmask()");
            exit(EXIT_FAILURE);
        }
        for (int j = 0; j < 5; j++)
        {
            write(1, "*", 1);
            usleep(usec);
        }
        write(1, "\n", 1);
        if (sigprocmask(SIG_UNBLOCK, &set, NULL) < 0)
        {
            perror("sigprocmask()");
            exit(EXIT_FAILURE);
        }
    }
    if (sigprocmask(SIG_SETMASK, &old_set, NULL) < 0)
    {
        perror("sigprocmask()");
        exit(EXIT_FAILURE);
    }
    if (signal(SIGINT, signal_handler_save) == SIG_ERR)
    {
        perror("Unable to unbind SIGINT");
        exit(EXIT_FAILURE);
    }    
    
    exit(EXIT_SUCCESS);
}