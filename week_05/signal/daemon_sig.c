#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <wait.h>
#include <syslog.h>
#include <string.h>
#include <errno.h>

FILE *file = NULL;

static void daemonize(void)
{
    pid_t pid = fork();
    if (pid < 0)
    {
        perror("fork1()");
        exit(EXIT_FAILURE);
    }
    if (pid > 0)
        exit(EXIT_SUCCESS);
    if (setsid() == (pid_t) -1)
    {
        perror("setsid()");
        exit(EXIT_FAILURE);
    }
    syslog(LOG_INFO, "setsid() success");

    pid = fork();
    if (pid < 0)
    {
        syslog(LOG_ERR, "fork2(): %s", strerror(errno));
        exit(EXIT_FAILURE);
    }
    if (pid > 0)
        exit(EXIT_SUCCESS);
    syslog(LOG_INFO, "Non transferred leader success");

    int fd = open("/dev/null", O_RDWR);
    dup2(fd, 0);
    dup2(fd, 1);
    dup2(fd, 2);
    if (fd > 2)
        close(fd);

    chdir("/");

    umask(0);
}

static void daemon_exit(int s)
{
    fclose(file);
    closelog();
    exit(EXIT_SUCCESS);
}

int main(int argc, char const *argv[])
{
    struct sigaction act;
    act.sa_handler = daemon_exit;
    sigemptyset(&act.sa_mask);
    sigaddset(&act.sa_mask, SIGINT);
    sigaddset(&act.sa_mask, SIGQUIT);
    sigaddset(&act.sa_mask, SIGTERM);
    act.sa_flags = 0;
    sigaction(SIGINT, &act, NULL);
    sigaction(SIGQUIT, &act, NULL);
    sigaction(SIGTERM, &act, NULL);

    openlog("mydaemon", LOG_PID, LOG_DAEMON);

    daemonize();
    syslog(LOG_INFO, "daemonize succuss");

    file = fopen("/tmp/out", "w");
    if (file == NULL)
    {
        syslog(LOG_ERR, "fopen(): %s", strerror(errno));
        exit(EXIT_FAILURE);
    }
    
    for (int i = 0; ; i++)
    {
        fprintf(file, "%d\n", i);
        fflush(file);
        sleep(1);
    }
}
