#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "uthash.h"

typedef struct ptr_st
{
    void *key;
    int i;
    UT_hash_handle hh;
}ptr_st;

int main(int argc, char const *argv[])
{
    ptr_st *users = NULL;
    ptr_st *item = malloc(sizeof *item);
    if (item == NULL)
        exit(-1);
    int a = 1;
    int *p = &a;
    printf("%p\n", p);
    item->key = p;
    item->i = 10;
    HASH_ADD_PTR(users, key, item);
    ptr_st *find_rel = NULL;    
    HASH_FIND_PTR(users, &p, find_rel);
    printf("%p %d\n", find_rel->key, find_rel->i);

    HASH_DEL(users, item);
    free(item);
    exit(0);
}
