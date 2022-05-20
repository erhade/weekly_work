#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utarray.h"


int main(int argc, char const *argv[])
{
    UT_array *str;
    utarray_new(str, &ut_str_icd);
    const char *s = NULL;
    char **p = NULL;

    s = "hello";
    utarray_push_back(str, &s);
    s = "world";
    utarray_push_back(str, &s);

    while ((p = utarray_next(str, p)))
    {
        printf("%s\n", *p);
    }

    utarray_free(str);

    exit(0);
}