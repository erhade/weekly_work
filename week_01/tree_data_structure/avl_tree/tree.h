#ifndef TREE_H__
#define TREE_H__

#include "utlist.h"
#include "utstack.h"

typedef struct tree_node_st
{
    int data;
    struct tree_node_st *left;
    struct tree_node_st *right;
}tree_node_st;

typedef struct node_st
{
    tree_node_st *ptree;
    struct node_st *next; 
}node_st;

tree_node_st *tree_new(int num);

tree_node_st *bst_new(int *arr, int len);

int bst_insert(tree_node_st **tree, int data);

tree_node_st *bst_find(tree_node_st *root, int key);

void preorder_recursion(tree_node_st *root, void (*process)(tree_node_st *cur_node));

void inorder_recursion(tree_node_st *root, void (*process)(tree_node_st *cur_node));

void postorder_recursion(tree_node_st *root, void (*process)(tree_node_st *cur_node));

int levelorder(tree_node_st *tree, void (*process)(tree_node_st *node));

int preorder(tree_node_st *tree, void (*process)(tree_node_st *node));

int inorder(tree_node_st *tree, void (*process)(tree_node_st *node));

int postorder(tree_node_st *tree, void (*process)(tree_node_st *node));

void tree_free(tree_node_st *root);

void avl_balance(tree_node_st **root);

int avl_delete(tree_node_st **tree, int key);

#endif