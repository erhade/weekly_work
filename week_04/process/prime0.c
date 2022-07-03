#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <math.h>
#include <stdbool.h>

#define LEFT    30000000
#define RIGHT   30000200

static bool is_prime(int num)
{
    for (int i = 2; i <= (int)sqrt((num * 1.0)); i++)
    {
        if (num % i == 0)
            return false;
    }
    return true;
}

int main(int argc, char const *argv[])
{
    for (int i = LEFT; i <= RIGHT; i++)
    {
        if (is_prime(i))
            printf("%d is prime\n", i);
    }

    exit(EXIT_SUCCESS);
}
