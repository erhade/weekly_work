#include <stdio.h>
#include <stdlib.h>

int main(int argc, char const *argv[])
{
    FILE *f1 = fopen("data", "r");
    FILE *f2 = fopen("data", "r");
    if (f1 == f2)
        printf("equal\n");
    printf("f1: %p, f2: %p\n", f1, f2);
    printf("f1: %d, f2: %d\n", f1->_fileno, f2->_fileno);
    exit(0);
}
