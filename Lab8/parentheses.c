#include "parentheses.h"
#include <stdlib.h>
#include <string.h>

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

void push_to_temp(parenthesis_t** stack, size_t opening, size_t closing, size_t* stack_size_ptr, size_t* stack_count_ptr)
{
    if (*stack_count_ptr == *stack_size_ptr) {
        *stack_size_ptr *= 2;
        *stack = realloc(*stack, *stack_size_ptr * sizeof(parenthesis_index_t));
    }
    (*stack)[*stack_count_ptr].opening_index = opening;
    (*stack)[*stack_count_ptr].closing_index = closing;
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
    parenthesis_index_t popped;
    parenthesis_index_t temp_popped;
    char counterpart;
    size_t stack_size = 2;
    size_t stack_count = 0;
    parenthesis_index_t* stack = (parenthesis_index_t*)malloc(sizeof(parenthesis_index_t) * stack_size);

    size_t temp_stack_size = 2;
    size_t temp_stack_count = 0;
    parenthesis_index_t* temp_stack = (parenthesis_index_t*)malloc(sizeof(parenthesis_index_t) * stack_size);

    size_t parentheses_malloc_size = 2;
    size_t parentheses_malloc_count = 0;
    parenthesis_t* parentheses_malloc = (parenthesis_t*)malloc(sizeof(parenthesis_t) * parentheses_malloc_size);

    size_t parentheses_count = 0;
    const char* p = str;
    size_t index = 0;

    while (*p != '\0') {
        if (*p == '{' || *p == '[' || *p == '(' || *p == '<') {
            push(&stack, *p, index, &stack_size, &stack_count);
        } else if (*p == '}' || *p == ')' || *p == ']' || *p == '>') {
            if (*p == '}') {
                counterpart = '{';
            } else if (*p == ')') {
                counterpart = '(';
            } else if (*p == ']') {
                counterpart = '[';
            } else {
                counterpart = '<';
            }
            while (1) {
                if (stack_count == 0) {
                    while (temp_stack_count != 0) {
                        temp_popped = pop(temp_stack, &temp_stack_count);
                        push(&stack, temp_popped.parenthesis, temp_popped.index, &stack_size, &stack_count);
                    }
                    break;
                }
                popped = pop(stack, &stack_count);
                if (popped.parenthesis == counterpart) {
                    push_to_temp(&parentheses_malloc, popped.index, index, &parentheses_malloc_size, &parentheses_malloc_count);
                    parentheses_count++;

                    while (temp_stack_count != 0) {
                        temp_popped = pop(temp_stack, &temp_stack_count);
                        push(&stack, temp_popped.parenthesis, temp_popped.index, &stack_size, &stack_count);
                    }
                    break;
                }
                else {
                    push(&temp_stack, popped.parenthesis, popped.index, &temp_stack_size, &temp_stack_count);
                }
            }
        }
        p++;
        index++;
    }
    qsort(parentheses_malloc, parentheses_malloc_count, sizeof(parenthesis_t), compare_stack);
    memcpy(parentheses, parentheses_malloc, sizeof(parenthesis_t) * max_size);
    free(stack);
    free(parentheses_malloc);
    free(temp_stack);
    if (parentheses_count > max_size) {
        parentheses_count = max_size;
    }
    return parentheses_count;
}

