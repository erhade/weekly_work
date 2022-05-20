#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tree.h"

#define NODE_NUM   6

static void pri_item(tree_node_st *cur)
{
    printf("%d ", cur->data);
    return;
}

int main(int argc, char const *argv[])
{
    tree_node_st *tree = NULL;
    int arr[] = {1, 2, 3, 7, 6, 5, 9, 8, 4};
    // tree = bst_new(arr, sizeof(arr) / sizeof(*arr));
    // tree = tree_new(NODE_NUM);
    for (int i = 0; i < (sizeof(arr) / sizeof(*arr)); i++)
    {
        bst_insert(&tree, arr[i]);
    }

    tree_node_st *find = bst_find(tree, 10);
    if (find != NULL)
        printf("find %d\n", find->data);
    else
        printf("can't find\n");

    avl_balance(&tree);

    int del_num = 5;
    avl_delete(&tree, del_num);

    if (tree != NULL)
    {
        preorder_recursion(tree, pri_item);
        putchar('\n');
        inorder_recursion(tree, pri_item);
        putchar('\n');
        postorder_recursion(tree, pri_item);
        putchar('\n');
        levelorder(tree, pri_item);
        putchar('\n');
        preorder(tree, pri_item);
        putchar('\n');
        inorder(tree, pri_item);
        putchar('\n');
        postorder(tree, pri_item);
        putchar('\n');
    }

    tree_free(tree);
    tree = NULL;
    exit(0);
}
