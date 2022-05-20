#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "cal.h"
#include "utstack.h"

typedef struct op_st
{
    char op;
    struct op_st *next;
}op_st;

typedef struct num_st
{
    int num;
    struct num_st *next;
}num_st;

cal_st *cal_new(const char *exp)
{
    cal_st *my_cal = malloc(sizeof *my_cal);
    if (my_cal == NULL)
        return NULL;
    my_cal->exp = exp ? exp : NULL;
    return my_cal;
}

void cal_free(cal_st **cal)
{
    if (cal == NULL || *cal == NULL)
        return;
    free(*cal);
    *cal = NULL;
    return;
}

static int is_op(const char c)
{
    if (c == '+' || c == '-' || c == '*' || c == '/' || c == '(' || c == ')')
        return 1;
    else
        return 0;
}

static char *Tokenization(const char *str, char Separator)
{
    if (str == NULL)
        return NULL;
    char *tokens = malloc((strlen(str) * 2) + 1);
    if (tokens == NULL)
        return NULL;
    char c;
    int count = 0;
    for (int i = 0; i < strlen(str); i++)
    {
        c = str[i];
        if (!(is_op(c) || isdigit(c)))
            continue;
        if (count == 0)
        {
            tokens[count++] = c;
            continue;
        } 
        if (isdigit(c))
        {
            if (is_op(tokens[count - 1]))
                tokens[count++] = Separator;
            tokens[count++] = c;
        }
        else
        {
            tokens[count++] = Separator;
            tokens[count++] = c;
        }
    }
    tokens[count++] = Separator;
    tokens[count] = '\0';
    return tokens;
}

static void op_push(op_st **op_stack, char c)
{
    op_st *new_op = NULL;
    new_op = malloc(sizeof *new_op);
    if (new_op == NULL)
        return;
    new_op->op = c;
    STACK_PUSH(*op_stack, new_op);
    return;
}

static void num_push(num_st **num_stack, int i)
{
    num_st *new_num = NULL;
    new_num = malloc(sizeof *new_num);
    if (new_num == NULL)
        return;
    new_num->num = i;
    STACK_PUSH(*num_stack, new_num);
    return;
}

static int calculate(char op, int a, int b)
{
    switch (op)
    {
    case '+':
        return a + b;
    case '-':
        return a - b;
    case '*':
        return a * b;
    default:
        return a / b;
    }
}

static void op_num_pop(op_st **op_stack, num_st **num_stack)
{
    op_st *op_pop = NULL; 
    STACK_POP(*op_stack, op_pop);
    num_st *num_pop1 = NULL;
    num_st *num_pop2 = NULL;
    STACK_POP(*num_stack, num_pop1);
    num_pop2 = STACK_TOP(*num_stack);
    int rel = calculate(op_pop->op, num_pop2->num, num_pop1->num);
    num_pop2->num = rel;
    free(op_pop);
    free(num_pop1);
    return;
}

int cal_process(cal_st *cal)
{
    if (cal == NULL)
        return -1;
    if (cal->exp == NULL)
        return -2;
    op_st *op_stack = NULL;
    num_st *num_stack = NULL;
    char Separator = 'A';
    char *tokens = Tokenization(cal->exp, Separator);
    char *p = NULL;
    char *head = tokens;
    char c;
    op_st *left_bracket = NULL;
    num_st *num_rel = NULL;
    
    while ((p = strchr(head, Separator)) != NULL)
    {
        *p = '\0';
        if (isdigit(*head))
        {
            num_push(&num_stack, atoi(head));
            // printf("%d\n", atoi(head));
        }
        else
        {
            c = *head;
            // printf("%c\n", c);
            if (c == '(')
                op_push(&op_stack, c);
            else if (c == ')')
            {
                while (STACK_TOP(op_stack)->op != '(')
                {
                    op_num_pop(&op_stack, &num_stack);
                }
                STACK_POP(op_stack, left_bracket);
                free(left_bracket);
                left_bracket = NULL;
            }
            else if (c == '+' || c == '-')
            {
                while (!STACK_EMPTY(op_stack) && STACK_TOP(op_stack)->op != '(')
                {
                    op_num_pop(&op_stack, &num_stack);
                }
                op_push(&op_stack, c);
            }
            else
            {
                while (!STACK_EMPTY(op_stack)
                &&STACK_TOP(op_stack)->op != '('   
                && STACK_TOP(op_stack)->op != '+' 
                && STACK_TOP(op_stack)->op != '-')
                {
                    op_num_pop(&op_stack, &num_stack);
                }
                op_push(&op_stack, c);
            }
        }
        head = p + 1;
    }

    free(tokens);

    while (!STACK_EMPTY(op_stack))
    {
        op_num_pop(&op_stack, &num_stack);
    }

    STACK_POP(num_stack, num_rel);
    cal->result = num_rel->num;
    free(num_rel);

    return 0;
}