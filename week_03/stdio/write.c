#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char const *argv[])
{
    char buf[5] = "hello";
    write(1, buf, 5);
    while (1)
    {
        /* code */
    }
    
    exit(0);
}
