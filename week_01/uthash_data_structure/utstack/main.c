#include <stdio.h>
#include <stdlib.h>

#include "utstack.h"

typedef struct stack_st
{
    int i;
    struct stack_st *next;
}stack_st;


int main(int argc, char const *argv[])
{
    stack_st *stack = NULL;
    stack_st *new_item = NULL;
    for (int i = 0; i < 5; i++)
    {
        new_item = malloc(sizeof *new_item);
        if (new_item == NULL)
            exit(-1);
        new_item->i = i;
        STACK_PUSH(stack, new_item);
    }

    int count;
    stack_st *current = NULL;
    STACK_COUNT(stack, current, count);
    printf("The stack has %d items.\n", count);

    while (!STACK_EMPTY(stack))
    {
        printf("%d\n", STACK_TOP(stack)->i);
        STACK_POP(stack, current);
        free(current);
    }
    
    exit(0);
}
