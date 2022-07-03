#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <wait.h>
#include <math.h>
#include <stdbool.h>

#define LEFT    30000000
#define RIGHT   30000200
#define N       3

static bool is_prime(int num)
{
    for (int i = 2; i <= (int)sqrt(num * 1.0); i++)
    {
        if (num % i == 0)
            return false;
    }
    return true;
}

int main(int argc, char const *argv[])
{
    pid_t pid;
    for (int i = 0; i < N; i++)
    {
        fflush(NULL);
        pid = fork();
        if (pid < 0)
        {
            for (int k = 0; k < i; k++)
                wait(NULL);
            
            perror("fork()");
            exit(EXIT_FAILURE);
        }
        if (pid == 0)
        {
            for (int j = LEFT; j <= RIGHT; j++)
            {
                if (j % N == i)
                {
                    if (is_prime(j))
                        printf("[%d] %d is prime\n", i + 1, j);
                }
            }
            exit(EXIT_SUCCESS);
        }
    }
    for (int i = 0; i < N; i++)
        wait(NULL);
    // sleep(1000);
    exit(EXIT_SUCCESS);
}