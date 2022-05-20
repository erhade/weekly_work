#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utarray.h"

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

static int cmp_by_s(const void *_a, const void *_b)
{
    const struct_complex_st *a = _a;
    const struct_complex_st *b = _b;
    return strcmp(a->s, b->s);
}

int main(int argc, char const *argv[])
{
    UT_array *nums;
    UT_icd complex_icd = {sizeof(struct_complex_st), NULL, copy, dtor};
    utarray_new(nums, &complex_icd);

    struct_complex_st s;
    s.i = 1;
    s.s = "world";
    utarray_push_back(nums, &s);
    s.i = 2;
    s.s = "hello";
    utarray_push_back(nums, &s);

    utarray_sort(nums, cmp_by_s);

    struct_complex_st *find_rel = NULL;
    struct_complex_st tmp;
    // tmp.i = 2;
    tmp.s = "hello";
    find_rel = utarray_find(nums, &tmp, cmp_by_s);
    if (find_rel != NULL)
        printf("find %d %s\n", find_rel->i, find_rel->s);

    struct_complex_st *p = NULL;
    while ((p = utarray_next(nums, p)))
    {
        printf("%d %s\n", p->i, p->s);
    }

    utarray_free(nums);

    exit(0);
}
