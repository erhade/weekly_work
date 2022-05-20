#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utlist.h"
#include "tree.h"

typedef struct list_node
{
    node_st *tree_node;
    struct list_node *next;
}list_node;

#define LINK(root, new_node, ch) (root->ch = new_node)

static void _ptr_link(node_st *root, node_st *new_node, char ch)
{
    if (ch == 'a')
        LINK(root, new_node, a);
    if (ch == 'b')
        LINK(root, new_node, b);
    if (ch == 'c')
        LINK(root, new_node, c);
    return;
}

static void enqueue(list_node **queue, node_st *node)
{
    if (queue == NULL || node == NULL)
        return;
    list_node *new_item = malloc(sizeof *new_item);
    if (new_item == NULL)
        return;
    new_item->tree_node = node;
    LL_APPEND(*queue, new_item);
    return;
}

static node_st *dequeue(list_node **queue)
{
    if (*queue == NULL || queue == NULL)
        return NULL;
    list_node *del_node = *queue;
    LL_DELETE(*queue, del_node);
    node_st *de_node = del_node->tree_node;
    free(del_node);
    return de_node;
}

static node_st* _node_init(void)
{
    node_st *new_node = malloc(sizeof *new_node);
    if (new_node == NULL)
        return NULL;
    new_node->a = NULL;
    new_node->b = NULL;
    new_node->c = NULL;
    new_node->str = NULL;
    new_node->count = 0;
    return new_node;
}

node_st *trie_tree_new(void)
{
    return _node_init();
}

int trie_tree_insert(node_st *tree, const char *word)
{
    if (tree == NULL)
        return -1;
    if (word == NULL)
        return -2;
    node_st *root = tree;
    node_st *new_node = NULL;
    for (int i = 0; i < (strlen(word) + 1); i++)
    {
        if (word[i] != 'a' && word[i] != 'b' && word[i] != 'c' && word[i] != '\0')
            continue;
        if (word[i] == '\0')
        {
            root->str = strdup(word);
            root->count++;
            break;
        }
        if (word[i] == 'a' && root->a != NULL)
        {
            root = root->a;
            continue;
        }
        if (word[i] == 'b' && root->b != NULL)
        {
            root = root->b;
            continue;
        }
        if (word[i] == 'c' && root->c != NULL)
        {
            root = root->c;
            continue;
        }
        new_node = _node_init();
        if (new_node == NULL)
            return -3;
        _ptr_link(root, new_node, word[i]);
        root = new_node;
    }
    return 0;
}

int trie_tree_levelorder(node_st *tree)
{
    if (tree == NULL)
        return -1;
    list_node *queue = NULL;
    node_st *cur = NULL;
    enqueue(&queue, tree);
    while (queue != NULL)
    {
        cur = dequeue(&queue);
        printf("%s %lu\n", cur->str ? cur->str : "null", cur->count);
        if (cur->a)
            enqueue(&queue, cur->a);
        if (cur->b)
            enqueue(&queue, cur->b);
        if (cur->c)
            enqueue(&queue, cur->c);
    }
    return 0;
}

void trie_tree_free(node_st *root)
{
    if (root == NULL)
        return;
    trie_tree_free(root->a);
    trie_tree_free(root->b);
    trie_tree_free(root->c);
    if (root->str)
        free(root->str);
    free(root);
}

int trie_tree_find(node_st *tree, const char *str, size_t *num)
{
    if (tree == NULL)
        return -1;
    if (num == NULL)
        return -2;
    node_st *root = tree;
    for (int i = 0; i < strlen(str); i++)
    {
        if (str[i] != 'a' && str[i] != 'b' && str[i] != 'c')
            return -3;
        if ((str[i] == 'a' && root->a == NULL) || (str[i] == 'b' && root->b == NULL) 
        || (str[i] == 'c' && root->c == NULL))
            return -3;
        if (str[i] == 'a')
            root = root->a;
        if (str[i] == 'b')
            root = root->b;
        if (str[i] == 'c')
            root = root->c;
    }
    *num = root->count;
    return 0;
}