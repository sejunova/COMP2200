#include "parentheses.h"
#include <stdlib.h>

size_t s_stack_count = 0;
size_t s_stack_size = 2;

void push(parenthesis_index_t** stack, char parenthesis, size_t index)
{
    if (s_stack_count == s_stack_size) {
        s_stack_size *= 2;
        *stack = realloc(*stack, s_stack_size * sizeof(parenthesis_index_t));
    }
    (*stack)[s_stack_count].parenthesis = parenthesis;
    (*stack)[s_stack_count].index = index;
    s_stack_count++;
}

parenthesis_index_t pop(parenthesis_index_t* stack)
{
    return stack[--s_stack_count];
}

int compare_stack(const void* a, const void* b)
{
    const parenthesis_t* parenthesis_a = a;
    const parenthesis_t* parenthesis_b = b;
    return (int)parenthesis_a->opening_index - (int)parenthesis_b->opening_index;
}

size_t get_matching_parentheses(parenthesis_t* parentheses, size_t max_size, const char* str)
{
    parenthesis_index_t* stack = (parenthesis_index_t*)malloc(sizeof(parenthesis_t) * s_stack_size);
    size_t parentheses_count = 0;
    const char* p = str;
    size_t index = 0;

    while (*p != '\0') {
        if (*p == '{' || *p == '[' || *p == '(' || *p == '<') {
            push(&stack, *p, index);
        }
        else if ((s_stack_count != 0) && ((*p == '}' && stack[s_stack_count - 1].parenthesis == '{') || (*p == ')' && stack[s_stack_count - 1].parenthesis == '(') || (*p == ']' && stack[s_stack_count - 1].parenthesis == '[') || (*p == '>' && stack[s_stack_count - 1].parenthesis == '<'))) {
            parentheses[parentheses_count].opening_index = pop(stack).index;
            parentheses[parentheses_count].closing_index = index;
            parentheses_count++;
            if (parentheses_count == max_size) {
                qsort(parentheses, parentheses_count, sizeof(parentheses), compare_stack);
                free(stack);
                return parentheses_count;
            }
        }
        p++;
        index++;
    }
    qsort(parentheses, parentheses_count, sizeof(parenthesis_t), compare_stack);
    free(stack);
    return parentheses_count;
}

