#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "uthash.h"

typedef struct key_st
{
    char a;
    int b;
}key_st;

typedef struct struct_st
{
    key_st key;
    UT_hash_handle hh;    
}struct_st;

int main(int argc, char const *argv[])
{
    struct_st *users = NULL;
    struct_st *item = malloc(sizeof *item);
    if (item == NULL)
        exit(-1);
    memset(item, 0, sizeof(*item));
    item->key.a = 'A';
    item->key.b = 1;
    HASH_ADD(hh, users, key, sizeof(key_st), item);

    struct_st l;
    memset(&l, 0, sizeof(l));
    l.key.a = 'A';
    l.key.b = 1;
    struct_st *find_rel = NULL;
    HASH_FIND(hh, users, &l.key, sizeof(key_st), find_rel);
    if (find_rel != NULL)
        printf("%c %d\n", find_rel->key.a, find_rel->key.b);

    exit(0);
}