#include <stdio.h>
#include <stdlib.h>

#include "tree.h"

static int enqueue(node_st **queue, tree_node_st *node)
{
    if (queue == NULL)
        return -1;
    if (node == NULL)
        return -2;
    node_st *new_node = malloc(sizeof *new_node);
    if (new_node == NULL)
        return -3;
    new_node->ptree = node;
    LL_APPEND(*queue, new_node);
    return 0;
}

static int dequeue(node_st **queue, tree_node_st **de_node)
{
    if (queue == NULL)
        return -1;
    if (*queue == NULL)  /* equal to count == 0 */
        return -2;
    if (de_node == NULL)
        return -3;
    node_st *del_node = *queue;
    LL_DELETE(*queue, del_node);
    *de_node = del_node->ptree;
    free(del_node);
    return 0;
}

static int queue_empty(const node_st *queue)
{
    return queue == NULL ? 1 : 0;
}

static int push(node_st **stack, tree_node_st *push_node)
{
    if (stack == NULL)
        return -1;
    if (push_node == NULL)
        return -2;
    node_st *new_node = malloc(sizeof *new_node);
    if (new_node == NULL)
        return -3;
    new_node->ptree = push_node;
    STACK_PUSH(*stack, new_node);
    return 0;
}

static int pop(node_st **stack, tree_node_st **pop_node)
{
    if (stack == NULL)
        return -1;
    if (*stack == NULL)
        return -2;
    if (pop_node == NULL)
        return -3;
    node_st *del_node = NULL;
    STACK_POP(*stack, del_node);
    *pop_node = del_node->ptree;
    free(del_node);
    return 0;
}

static int stack_empty(const node_st *stack)
{
    return STACK_EMPTY(stack);
}

tree_node_st *tree_new(int num)
{
    if (num <= 0)
        return NULL;
    int count = 1;
    node_st *queue = NULL;
    tree_node_st *tree = NULL;
    tree_node_st *root_node = NULL;
    tree_node_st *left_node = NULL;
    tree_node_st *right_node = NULL;
    root_node = malloc(sizeof *root_node);
    if (root_node == NULL)
        return NULL;
    root_node->data = 1;
    tree = root_node;
    enqueue(&queue, root_node);
    for (int i = 2; i <= num; i++)
    {
        dequeue(&queue, &root_node);
        left_node = malloc(sizeof *left_node);
        if (left_node == NULL)
            return NULL;
        left_node->data = i;
        root_node->left = left_node;
        enqueue(&queue, left_node);
        if (i + 1 <= num)
        {
            right_node = malloc(sizeof *right_node);
            if (right_node == NULL)
                return NULL;
            right_node->data = ++i;
            root_node->right = right_node;
            enqueue(&queue, right_node);
        }
    }
    while (queue != NULL)
    {
        dequeue(&queue, &root_node);
    }
    return tree;
}

tree_node_st *bst_new(int *arr, int len)
{
    if (arr == NULL || len <= 0)
        return NULL;
    tree_node_st *tree = NULL;
    tree_node_st *new_node = malloc(sizeof *new_node);
    if (new_node == NULL)
        return NULL;
    new_node->data = arr[0];
    tree = new_node;
    tree_node_st *root = NULL;
    tree_node_st *prev = NULL;
    int direction;
    for (int i = 1; i < len; i++)
    {
        root = tree;
        new_node = malloc(sizeof *new_node);
        if (new_node == NULL)
            return NULL;
        new_node->data = arr[i];
        // printf("new_node: %d\n", new_node->data);
        while (root != NULL)
        {
            // printf("root: %d\n", root->data);
            prev = root;
            if (arr[i] >= root->data)
            {
                root = root->right;
                direction = 0;
            }
            else
            {
                root = root->left;
                direction = 1;
            }
        }
        if (direction == 0)
            prev->right = new_node;
        else
            prev->left = new_node; 
    }
    return tree;
}

int bst_insert(tree_node_st **tree, int data)
{
    tree_node_st *new_node = malloc(sizeof *new_node);
    if (new_node == NULL)
        return -1;
    new_node->data = data;

    tree_node_st **root = tree;
    while (*root != NULL)
    {
        if (data >= (*root)->data)
            root = &(*root)->right;
        else
            root = &(*root)->left;
    }
    *root = new_node;
    return 0;
}

tree_node_st *bst_find(tree_node_st *root, int key)
{
    if (root == NULL)
        return NULL;
    if (key == root->data)
        return root;
    if (key > root->data)
        return bst_find(root->right, key);
    if (key < root->data)
        return bst_find(root->left, key);
}

tree_node_st **_bst_find(tree_node_st **root, int key)
{
    if (*root == NULL || (*root)->data == key)
        return root;
    if ((*root)->data > key)
        _bst_find(&(*root)->left, key);
    if ((*root)->data < key)
        _bst_find(&(*root)->right, key);
}

void preorder_recursion(tree_node_st *root, void (*process)(tree_node_st *cur_node))
{
    if (root == NULL)
        return;
    process(root);
    preorder_recursion(root->left, process);
    preorder_recursion(root->right, process);
}

void inorder_recursion(tree_node_st *root, void (*process)(tree_node_st *cur_node))
{
    if (root == NULL)
        return;
    inorder_recursion(root->left, process);
    process(root);
    inorder_recursion(root->right, process);
}

void postorder_recursion(tree_node_st *root, void (*process)(tree_node_st *cur_node))
{
    if (root == NULL)
        return;
    postorder_recursion(root->left, process);
    postorder_recursion(root->right, process);
    process(root);
}

void tree_free(tree_node_st *root)
{
    if (root != NULL)
        return;
    tree_free(root->left);
    tree_free(root->right);
    free(root);
}

int levelorder(tree_node_st *tree, void (*process)(tree_node_st *node))
{
    if (tree == NULL)
        return -1;
    if (process == NULL)
        return -2;
    node_st *queue = NULL;
    tree_node_st *root = NULL;
    tree_node_st *left = NULL;
    tree_node_st *right = NULL;
    enqueue(&queue, tree);
    while (!queue_empty(queue))
    {
        dequeue(&queue, &root);
        process(root);
        if (root->left != NULL)
            enqueue(&queue, root->left);
        if (root->right != NULL)
            enqueue(&queue, root->right);
    }
    return 0;
}

int preorder(tree_node_st *tree, void (*process)(tree_node_st *node))
{
    if (tree == NULL)
        return -1;
    if (process == NULL)
        return -2;
    node_st *stack = NULL;
    tree_node_st *root = tree;
    while (root != NULL || !stack_empty(stack))
    {
        while (root != NULL)
        {
            process(root);
            push(&stack, root);
            root = root->left;
        }
        if (!stack_empty(stack))
        {
            pop(&stack, &root);
            root = root->right;
        }
    }
    return 0;
}

int inorder(tree_node_st *tree, void (*process)(tree_node_st *node))
{
    if (tree == NULL)
        return -1;
    if (process == NULL)
        return -2;
    node_st *stack = NULL;
    tree_node_st *root = tree;
    while (root != NULL || !stack_empty(stack))
    {
        while (root != NULL)
        {
            push(&stack, root);
            root = root->left;
        }
        if (!stack_empty(stack))
        {
            pop(&stack, &root);
            process(root);
            root = root->right;
        }
    }
    return 0;
}

int postorder(tree_node_st *tree, void (*process)(tree_node_st *node))
{
    if (tree == NULL)
        return -1;
    if (process == NULL)
        return -2;
    node_st *stack = NULL;
    tree_node_st *root = tree;
    tree_node_st *pre = NULL;
    push(&stack, root);
    while (!stack_empty(stack))
    {
        root = stack->ptree;
        if ((root->left == NULL && root->right == NULL) 
        || (pre != NULL && (pre == root->left || pre == root->right)))
        {
            pop(&stack, &root);
            process(root);
            pre = root;
        }
        else
        {
            if (root->right)
                push(&stack, root->right);
            if (root->left)
                push(&stack, root->left);
        }
    }
    return 0;
}

static int high(tree_node_st *root)
{
    if (root == NULL)
        return 0;
    int h1 = high(root->left);
    int h2 = high(root->right);
    return h1 >= h2 ? h1 + 1 : h2 + 1;
}

static void turn_left(tree_node_st **tree)
{
    tree_node_st *old_root = *tree;
    *tree = old_root->right;
    old_root->right = NULL;
    tree_node_st *new_root = *tree;
    while (new_root->left != NULL)
    {
        new_root = new_root->left;
    }
    new_root->left = old_root;
    return;
}

static void turn_right(tree_node_st **tree)
{
    tree_node_st *old_root = *tree;
    tree_node_st *new_root = old_root->left;
    *tree = new_root;
    old_root->left = NULL;
    while (new_root->right != NULL)
    {
        new_root = new_root->right;
    }
    new_root->right = old_root;
    return;
}

void avl_balance(tree_node_st **root)
{
    if (*root == NULL)
        return;
    int sub;
    while (1)
    {
        sub = high((*root)->left) - high((*root)->right);
        if (sub >= -1 && sub <= 1)
            break;
        if (sub <-1)
            turn_left(root);
        if (sub > 1)
            turn_right(root);
    }
    avl_balance(&(*root)->left);
    avl_balance(&(*root)->right);
}

int avl_delete(tree_node_st **tree, int key)
{
    if (tree == NULL)
        return -1;
    if (*tree == NULL)
        return -2;
    tree_node_st **root = tree;
    #if 0
    while (*root != NULL && (*root)->data != key)
    {
        if (key < (*root)->data)
            root = &(*root)->left;
        if (key > (*root)->data)
            root = &(*root)->right;
    }
    #endif
    root = _bst_find(root, key);
    if (*root == NULL)
        return -3;
    tree_node_st *del_node = *root;
    tree_node_st *left = del_node->left;
    tree_node_st *right = del_node->right;
    if (left == NULL)
    {
        *root = right;
    }
    else
    {
        *root = left;
        tree_node_st *cur = left;
        while (cur->right != NULL)
            cur = cur->right;
        cur->right = right;
    }
    free(del_node);
    avl_balance(tree);
    return 0;
}