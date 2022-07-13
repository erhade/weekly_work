#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char const *argv[])
{
    unsigned long num = 0;
    time_t ti;
    ti = time(NULL);
    ti += 5;
    while (time(NULL) < ti)
        num++;
    printf("time:\n%lu\n", num);
    exit(EXIT_SUCCESS);
}
