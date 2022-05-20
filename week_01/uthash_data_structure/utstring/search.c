#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utstring.h"

int main(int argc, char const *argv[])
{
    UT_string *s;
    int pos;
    utstring_new(s);
    utstring_printf(s, "ABC ABCDAB ABCDABCDABDE");

    long *kmp_table = malloc(sizeof(long) * (strlen("ABC") + 1));
    if (kmp_table == NULL)
        exit(-1);
    
    _utstring_BuildTable("ABC", strlen("ABC"), kmp_table);

    int offset = 3;
    pos = _utstring_find(utstring_body(s) + offset, 
    utstring_len(s) - offset, 
    "ABC", 
    strlen("ABC"), 
    kmp_table) + offset;

    printf("%d\n", pos);

    free(kmp_table);
    utstring_free(s);
    exit(0);
}
