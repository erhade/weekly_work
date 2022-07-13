#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>

#include "anytimer.h"

static void func(const char *str)
{
    puts(str);
}

int main(int argc, char const *argv[])
{
    int job1, job2, job3;
    puts("begin");

    job1 = anytimer_init(1, func, "aaa", false);
    if (job1 < 0)
    {
        fprintf(stderr, "anytimer_init() fail %s\n", strerror(-job1));
        exit(EXIT_FAILURE);
    }

    job2 = anytimer_init(2, func, "bbb", true);
    if (job2 < 0)
    {
        fprintf(stderr, "anytimer_init() fail %s\n", strerror(-job2));
        exit(EXIT_FAILURE);
    }
    anytimer_stop(job2);

    job3 = anytimer_init(3, func, "ccc", false);
    if (job3 < 0)
    {
        fprintf(stderr, "anytimer_init() fail %s\n", strerror(-job2));
        exit(EXIT_FAILURE);
    }
    anytimer_cancel(job3);

    puts("end");

    anytimer_wait(job3);
    anytimer_wait(job1);

    anytimer_resume(job2);
    anytimer_wait(job2);

    exit(EXIT_SUCCESS);
}
