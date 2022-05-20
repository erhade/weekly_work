#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tree.h"

#define BUF_LEN    10

int main(int argc, char const *argv[])
{
    FILE *file = fopen("word.txt", "r");
    char line_buf[BUF_LEN];
    char *p = NULL;
    node_st *tree = trie_tree_new();
    if (file == NULL)
    {
        perror("cant't open ");
        exit(-1);
    }
    while ((fgets(line_buf, BUF_LEN, file)) != NULL)
    {
        p = strchr(line_buf, '\n');
        *p = '\0';
        trie_tree_insert(tree, line_buf);
    }

    fclose(file);

    trie_tree_levelorder(tree);

    size_t count;
    trie_tree_find(tree, "caaba", &count);
    printf("count is %lu\n", count);

    trie_tree_free(tree);
    tree = NULL;

    exit(0);
}
