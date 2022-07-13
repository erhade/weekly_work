#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <stdbool.h>

static volatile bool flag = true;

static void alarm_handler(int s)
{
    flag = false;
}

int main(int argc, char const *argv[])
{
    unsigned long num = 0;
    signal(SIGALRM, alarm_handler);
    alarm(5);
    while (flag)
        num++;
    printf("time:\n%lu\n", num);
    exit(EXIT_SUCCESS);
}
