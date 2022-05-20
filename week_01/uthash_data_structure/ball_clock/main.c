#include <stdio.h>
#include <stdlib.h>

#include "utlist.h"
#include "utstack.h"

#define QUEUE_LEN       27
#define ONE_HOUR_LEN    11
#define FIVE_MINUTE_LEN 11
#define ONE_MINUTE_LEN  4    

typedef struct node_st
{
    int i;
    struct node_st *next;
}node_st;

static void enqueue(node_st **queue, node_st *new_node)
{
    LL_APPEND(*queue, new_node);
    return;
}

static node_st *dequeue(node_st **queue)
{
    node_st *del = *queue;
    LL_DELETE(*queue, del);
    return del;
}

static int stack_count(node_st *stack)
{
    int counter;
    node_st *elt = NULL;
    LL_COUNT(stack, elt, counter);
    return counter;
}

static int equal(node_st *queue)
{
    node_st *current = NULL;
    int k = 0;
    LL_FOREACH(queue, current)
    {
        k++;
        if (current->i != k)
            return 0;
    }
    return 1;
}

int main(int argc, char const *argv[])
{
    node_st *one_hour_stack = NULL;
    node_st *five_minute_stack = NULL;
    node_st *one_minute_stack = NULL;
    node_st *queue = NULL;
    node_st *new_node = NULL;
    node_st *de_item = NULL;
    node_st *pop_item = NULL;
    int count = 0;
    
    for (int i = 0; i < QUEUE_LEN; i++)
    {
        new_node = malloc(sizeof *new_node);
        if (new_node == NULL)
            exit(-1);
        new_node->i = i+1;
        enqueue(&queue, new_node);
    }

    while (1)
    {
        de_item = dequeue(&queue);
        count++;
        if (stack_count(one_minute_stack) != ONE_MINUTE_LEN)
        {
            STACK_PUSH(one_minute_stack, de_item);
        }
        else 
        {
            for (int i = 0; i < ONE_MINUTE_LEN; i++)
            {
                STACK_POP(one_minute_stack, pop_item);
                enqueue(&queue, pop_item);
            }
            if (stack_count(five_minute_stack) != FIVE_MINUTE_LEN)
            {
                STACK_PUSH(five_minute_stack, de_item);
            }
            else
            {
                for (int i = 0; i < FIVE_MINUTE_LEN; i++)
                {
                    STACK_POP(five_minute_stack, pop_item);
                    enqueue(&queue, pop_item);
                }
                if (stack_count(one_hour_stack) != ONE_HOUR_LEN)
                {
                    STACK_PUSH(one_hour_stack, de_item);
                }
                else
                {
                    for (int i = 0; i < ONE_HOUR_LEN; i++)
                    {
                        STACK_POP(one_hour_stack, pop_item);
                        enqueue(&queue, pop_item);
                    }
                    enqueue(&queue, de_item);
                    if (equal(queue))
                        break;
                }
            }
        }
    }
    
    printf("%d\n", count);

    node_st *current = NULL;
    node_st *tmp = NULL;
    LL_FOREACH_SAFE(queue, current, tmp)
    {
        LL_DELETE(queue, current);
        free(current);
    }
    
    exit(0);
}
