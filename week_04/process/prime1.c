#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <wait.h>
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
    pid_t pid;
    for (int i = LEFT; i <= RIGHT; i++)
    {
        pid = fork();
        if (pid < 0)
        {
            perror("fork()");
            exit(EXIT_FAILURE);
        }
        if (pid == 0)
        {
            if (is_prime(i))
                printf("%d is prime\n", i);
            exit(EXIT_SUCCESS);
        }
    }
    for (int i = LEFT; i <= RIGHT; i++)
        wait(NULL);
    
    sleep(1000);
    exit(EXIT_SUCCESS);
}
