#ifndef TREE_H__
#define TREE_H__

#include <stddef.h>

/* If the dictionary is 26 case insensitive letters, 
you should use pointer arrays instead of macros. */
/* 
typedef struct node_st
{
    struct node_st *ch[26];
    char *str;
    int count;
}node_st;

node_st *root;
(root->ch)[letter - 'a'] = new_node;
*/

typedef struct node_st
{
    struct node_st *a;
    struct node_st *b;
    struct node_st *c;
    char *str;
    size_t count;
}node_st;

node_st *trie_tree_new(void);

int trie_tree_insert(node_st *tree, const char *word);

int trie_tree_levelorder(node_st *tree);

void trie_tree_free(node_st *root);

int trie_tree_find(node_st *tree, const char *str, size_t *num);

#endif