#ifndef TODO_LIST_H
#define TODO_LIST_H

#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct node {
    int32_t priority;
    char* task;
    struct node* next;
} node_t;

typedef struct todo_list {
    node_t* head;
    size_t cur_size;
    size_t max_size;
} todo_list_t;

todo_list_t* init_todo_list_malloc(size_t max_size);

void dispose(todo_list_t* todo_list);

bool add_todo(todo_list_t* todo_list, const int32_t priority, const char* task);

bool complete_todo(todo_list_t* todo_list);

const char* peek_or_null(todo_list_t* todo_list);

size_t get_count(todo_list_t* todo_list);

bool is_empty(todo_list_t* todo_list);

void print_task(todo_list_t* todo_list);

#endif /* TODO_LIST_H */
