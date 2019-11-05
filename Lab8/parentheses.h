#include <stdio.h>
#ifndef PARENTHESES_H
#define PARENTHESES_H

typedef struct {
    size_t opening_index;
    size_t closing_index;
} parenthesis_t;

typedef struct {
    char parenthesis;
    size_t index;
} parenthesis_index_t;

size_t get_matching_parentheses(parenthesis_t* parentheses, size_t max_size, const char* str);

#endif /* PARENTHESES_H */

