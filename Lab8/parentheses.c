#include "parentheses.h"
#include <stdlib.h>

void push(parenthesis_index_t** stack, char parenthesis, size_t index, size_t* stack_size_ptr, size_t* stack_count_ptr)
{
    if (*stack_count_ptr == *stack_size_ptr) {
        *stack_size_ptr *= 2;
        *stack = realloc(*stack, *stack_size_ptr * sizeof(parenthesis_index_t));
    }
    (*stack)[*stack_count_ptr].parenthesis = parenthesis;
    (*stack)[*stack_count_ptr].index = index;
    (*stack_count_ptr)++;
}

parenthesis_index_t pop(parenthesis_index_t* stack, size_t* stack_count_ptr)
{
    return stack[--*stack_count_ptr];
}

int compare_stack(const void* a, const void* b)
{
    const parenthesis_t* parenthesis_a = a;
    const parenthesis_t* parenthesis_b = b;
    return (int)parenthesis_a->opening_index - (int)parenthesis_b->opening_index;
}

size_t get_matching_parentheses(parenthesis_t* parentheses, size_t max_size, const char* str)
{
    size_t stack_size = 2;
    size_t stack_count = 0;
    parenthesis_index_t* stack = (parenthesis_index_t*)malloc(sizeof(parenthesis_index_t) * stack_size);
    size_t parentheses_count = 0;
    const char* p = str;
    size_t index = 0;

    while (*p != '\0') {
        if (*p == '{' || *p == '[' || *p == '(' || *p == '<') {
            push(&stack, *p, index, &stack_size, &stack_count);
        } else if ((stack_count != 0) && ((*p == '}' && stack[stack_count - 1].parenthesis == '{') || (*p == ')' && stack[stack_count - 1].parenthesis == '(') || (*p == ']' && stack[stack_count - 1].parenthesis == '[') || (*p == '>' && stack[stack_count - 1].parenthesis == '<'))) {
            parentheses[parentheses_count].opening_index = pop(stack, &stack_count).index;
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

