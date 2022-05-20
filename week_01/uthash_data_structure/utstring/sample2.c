#include <stdio.h>
#include <stdlib.h>

#include "utstring.h"

int main(int argc, char const *argv[])
{
    UT_string str1, str2;
    utstring_init(&str1);
    utstring_init(&str2);

    int a = 1;
    int b = 2;
    utstring_printf(&str1, "hello %d ", a);
    utstring_printf(&str1, "world %d", a);

    utstring_printf(&str2, "hello %d ", b);
    utstring_printf(&str2, "world %d", b);

    printf("%s\n", utstring_body(&str1));
    printf("%s\n", utstring_body(&str2));

    utstring_concat(&str1, &str2);

    utstring_clear(&str2);

    printf("%s\n", utstring_body(&str1));
    printf("%s\n", utstring_body(&str2));

    printf("str2 len is %lu\n", utstring_len(&str1));
    printf("str2 len is %lu\n", utstring_len(&str2));

    utstring_done(&str1);
    utstring_done(&str2);

    exit(0);
}
