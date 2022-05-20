#include <stdio.h>
#include <stdlib.h>

#include "utarray.h"

#define ARRAY_LEN   10

int main(int argc, char const *argv[])
{
    UT_array *nums;
    utarray_new(nums, &ut_int_icd);
    utarray_reserve(nums, ARRAY_LEN);
    for (int i = 0; i < ARRAY_LEN; i++)
    {
        utarray_push_back(nums, &i);
    }

    int *pos = NULL;
    while ((pos = utarray_prev(nums, pos)))
    {
        printf("%d ", *pos);
    }
    putchar('\n');

    utarray_free(nums);
    
    exit(0);
}
