#include <stdio.h>
#include <stdlib.h>

static void func(void)
{
    printf("func is called.\n");
}

int i = 4;

int main(int argc, char const *argv[])
{
    func();
    i = 1;
    printf("hello\n");
    printf("world\n");
    printf("%d\n", i);
    exit(0);
}
