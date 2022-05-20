#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utlist.h"

typedef struct node_st
{
    int i;
    struct node_st *prev;
    struct node_st *next;
}node_st;


int main(int argc, char const *argv[])
{
    node_st *list1 = NULL;
    node_st *list2 = NULL;

    node_st *new_item = NULL;
    for (int i = 20; i < 25; i++)
    {
        new_item = malloc(sizeof *new_item);
        if (new_item == NULL)
            exit(-1);
        new_item->i = i;
        DL_APPEND(list1, new_item);
    }

    for (int i = 0; i < 5; i++)
    {
        new_item = malloc(sizeof *new_item);
        if (new_item == NULL)
            exit(-1);
        new_item->i = i+10;
        DL_APPEND(list2, new_item);
    }

    node_st *current = NULL;
    DL_FOREACH(list1, current)
    {
        printf("%d\n", current->i);
    }
    
    putchar('\n');

    DL_FOREACH(list2, current)
    {
        printf("%d\n", current->i);
    }

    DL_CONCAT(list1, list2);

    putchar('\n');

    DL_FOREACH(list1, current)
    {
        printf("%d\n", current->i);
    }

    node_st *tmp = NULL;
    DL_FOREACH_SAFE(list1, current, tmp)
    {
        DL_DELETE(list1, current);
        free(current);
    }

    list2 = NULL;

    exit(0);
}
