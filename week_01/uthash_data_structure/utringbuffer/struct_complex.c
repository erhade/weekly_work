#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utringbuffer.h"

#define BUF_LEN    2

typedef struct struct_complex_st
{
    int i;
    char *s;
}struct_complex_st;

static void copy(void *_dst, const void *_src)
{
    struct_complex_st *dst = _dst;
    const struct_complex_st *src = _src;
    dst->i = src->i;
    dst->s = src->s ? strdup(src->s) : NULL;
    return; 
}

static void dtor(void *_dst)
{
    struct_complex_st *dst = _dst;
    if (dst->s)
        free(dst->s);
    return;
}

int main(int argc, char const *argv[])
{
    UT_ringbuffer *history;
    UT_icd complex_icd = {sizeof(struct_complex_st), NULL, copy, dtor};
    utringbuffer_new(history, BUF_LEN, &complex_icd);

    struct_complex_st tmp;
    tmp.i = 1;
    tmp.s = "A";
    utringbuffer_push_back(history, &tmp);

    tmp.i = 2;
    tmp.s = "B";
    utringbuffer_push_back(history, &tmp);

    tmp.i = 3;
    tmp.s = "C";
    utringbuffer_push_back(history, &tmp);

    struct_complex_st *p = NULL;
    while ((p = utringbuffer_next(history, p)))
    {
        printf("%d %s\n", p->i, p->s);
    }

    utringbuffer_free(history);

    exit(0);
}
