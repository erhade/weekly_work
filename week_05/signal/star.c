#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

static void int_handler(int num)
{
    char str[10];
    for (int i = 0; i < 5; i++)
    {
        snprintf(str, 10, "%d", i);
        write(1, str, 1);
        sleep(1);
    }
}

int main(int argc, char const *argv[])
{
    // signal(SIGINT, SIG_IGN);
    signal(SIGINT, int_handler);
    for (int i = 0; i < 100; i++)
    {
        write(1, "*", 1);
        sleep(1);
    }
    exit(EXIT_SUCCESS);
}
