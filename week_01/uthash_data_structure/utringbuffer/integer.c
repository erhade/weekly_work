#include <stdio.h>
#include <stdlib.h>

#include "utringbuffer.h"

#define BUF_LEN    5

int main(int argc, char const *argv[])
{
    UT_ringbuffer *history;
    utringbuffer_new(history, BUF_LEN, &ut_int_icd);
    for (int i = 0; i < 7; i++)
    {
        utringbuffer_push_back(history, &i);
    }

    int *p = NULL;
    while ((p = utringbuffer_next(history, p)))
    {
        printf("%d ", *p);
    }
    putchar('\n');

    utringbuffer_free(history);
    
    exit(0);
}
