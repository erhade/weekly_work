#include <stdio.h>
#include <stdlib.h>

int main(int argc, char const *argv[])
{
    char *value = getenv("PATH");
    if (value != NULL)
        puts(value);
    exit(EXIT_SUCCESS);
}
