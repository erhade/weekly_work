#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utlist.h"

#define BUF_LEN    20

typedef struct node_st
{
    char name[BUF_LEN];
    struct node_st *prev;
    struct node_st *next;
}node_st;

static int cmp(const node_st *a, const node_st *b)
{
    return strcmp(a->name, b->name);
}

int main(int argc, char const *argv[])
{
    node_st *list = NULL;
    char buf_line[BUF_LEN];
    FILE *file = fopen("test11.dat", "r");
    if (file == NULL)
    {
        perror("can't open ");
        exit(-1);
    }
    node_st *new_item = NULL;
    while (fgets(buf_line, BUF_LEN, file) != NULL)
    {
        new_item = malloc(sizeof *new_item);
        if (new_item == NULL)
            exit(-2);
        strcpy(new_item->name, buf_line);
        // DL_PREPEND(list, new_item);
        // DL_APPEND(list, new_item);
        DL_INSERT_INORDER(list, new_item, cmp);
    }

    fclose(file);

    
    node_st *find_rel = NULL;
    node_st wes;
    strcpy(wes.name, "WES\n");
    DL_SEARCH(list, find_rel, &wes, cmp);
    printf("find %s\n", find_rel->name);

    node_st *alice = malloc(sizeof *alice);
    if (alice == NULL)
        exit(-3);
    strcpy(alice->name, "ALICE\n");
    DL_REPLACE_ELEM(list, find_rel, alice);
    free(find_rel);

    node_st willbrd;
    strcpy(willbrd.name, "WILLBRD");
    DL_LOWER_BOUND(list, find_rel, &willbrd, cmp);
    printf("\n%s\n", find_rel->name);

    node_st *elt = NULL;
    DL_FOREACH(list, elt)
    {
        printf("%s", elt->name);
    }

    int count;
    DL_COUNT(list, elt, count);
    printf("The list has %d nodes.\n", count);

    node_st *tmp = NULL;
    DL_FOREACH_SAFE(list, elt, tmp)
    {
        DL_DELETE(list, elt);
        free(elt);
    }

    exit(0);
}
