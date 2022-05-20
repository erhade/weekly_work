#include <stdio.h>
#include <stdlib.h>

#include "utarray.h"

typedef struct int_pair_st
{
    int a;
    int b;
}int_pair_st;

int main(int argc, char const *argv[])
{
    UT_array *nums;
    UT_icd int_pair_icd = {sizeof(int_pair_st), NULL, NULL, NULL};
    utarray_new(nums, &int_pair_icd);

    int_pair_st s;
    s.a = 1;
    s.b = 2;
    utarray_push_back(nums, &s);
    s.a = 3;
    s.b = 4;
    utarray_push_back(nums, &s);

    int_pair_st *p = NULL;
    while ((p = utarray_next(nums, p)))
    {
        printf("%d %d\n", p->a, p->b);
    }

    utarray_free(nums);

    exit(0);
}
