#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "uthash.h"

#define NAME_LEN    20

typedef struct string_arr_key_st
{
    char name[NAME_LEN];
    UT_hash_handle hh;
}string_arr_key_st;

int main(int argc, char const *argv[])
{
    string_arr_key_st *users = NULL;
    const char *names[] = {"Alice", "Bob", "Trudy"};
    string_arr_key_st *new_item = NULL;
    for (int i = 0; i < 3; i++)
    {
        new_item = malloc(sizeof *new_item);
        if (new_item == NULL)
            exit(-1);
        strcpy(new_item->name, names[i]);
        HASH_ADD_STR(users, name, new_item);
    }

    string_arr_key_st *find_rel = NULL;
    HASH_FIND_STR(users, "Bob", find_rel);
    printf("find %s.\n", find_rel->name);

    string_arr_key_st *current;
    for (current = users; current != NULL; current = current->hh.next)
    {
        printf("%s.\n", current->name);
    }

    string_arr_key_st *tmp;
    HASH_ITER(hh, users, current, tmp)
    {
        HASH_DEL(users, current);
        free(current);
    }

    exit(0);
}
