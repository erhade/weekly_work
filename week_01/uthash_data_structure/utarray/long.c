#include <stdio.h>
#include <stdlib.h>

#include "utarray.h"

int main(int argc, char const *argv[])
{
    UT_array *nums;
    UT_icd long_icd = {sizeof(long), NULL, NULL, NULL};
    utarray_new(nums, &long_icd);
    for (long i = 0; i < 10; i++)
    {
        utarray_push_back(nums, &i);
    }

    long *p = NULL;
    while ((p = utarray_next(nums, p)))
    {
        printf("%ld ", *p);
    }
    putchar('\n');

    utarray_free(nums);    
    exit(0);
}
