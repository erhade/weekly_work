#include <stdio.h>
#include <stdlib.h>
#include <setjmp.h>

jmp_buf save;

static void d(void)
{
    printf("[%s] start\n", __FUNCTION__);
    longjmp(save, 1);
    printf("[%s] end\n", __FUNCTION__);
}

static void c(void)
{
    printf("[%s] start\n", __FUNCTION__);
    printf("[%s] called d()\n", __FUNCTION__);
    d();
    printf("[%s] end\n", __FUNCTION__);
}

static void b(void)
{
    printf("[%s] start\n", __FUNCTION__);
    printf("[%s] called c()\n", __FUNCTION__);
    c();
    printf("[%s] end\n", __FUNCTION__);
}

static void a(void)
{
    printf("[%s] start\n", __FUNCTION__);
    printf("[%s] called b()\n", __FUNCTION__);
    b();
    printf("[%s] end\n", __FUNCTION__);
}

int main(int argc, char const *argv[])
{
    int ret;
    printf("[%s] start\n", __FUNCTION__);
    printf("[%s] called a()\n", __FUNCTION__);
    ret = setjmp(save);
    if (ret == 0)
        a();
    printf("[%s] end\n", __FUNCTION__);
    exit(EXIT_SUCCESS);
}
