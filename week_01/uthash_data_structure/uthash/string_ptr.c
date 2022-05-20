#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "uthash.h"

#define NAME_LEN    20

typedef struct string_ptr_st
{
    const char *name;
    UT_hash_handle hh;
}string_ptr_st;

int main(int argc, char const *argv[])
{
    string_ptr_st *users = NULL;
    const char *names[] = {"Alice", "Bob", "Trudy", NULL};

    string_ptr_st *new_item = NULL;
    for (int i = 0; names[i]; i++)
    {
        new_item = malloc(sizeof *new_item);
        if (new_item == NULL)
            exit(-1);
        new_item->name = names[i];
        HASH_ADD_KEYPTR(hh, users, new_item->name, strlen(new_item->name), 
        new_item);
    }

    string_ptr_st *current = NULL;
    for (current = users; current != NULL; current = current->hh.next)
    {
        printf("%s\n", current->name);
    }

    string_ptr_st *find_rel = NULL;
    HASH_FIND_STR(users, "Trudy", find_rel);
    printf("%s\n", find_rel->name);

    string_ptr_st *tmp = NULL;
    HASH_ITER(hh, users, current, tmp)
    {
        HASH_DEL(users, current);
        free(current);
    }
    
    exit(0);
}