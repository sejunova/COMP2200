#include "todo_list.h"

todo_list_t* init_todo_list_malloc(size_t max_size)
{
    todo_list_t* to_do_list_ptr;
    to_do_list_ptr = (todo_list_t*)malloc(sizeof(todo_list_t));
    to_do_list_ptr->cur_size = 0;
    to_do_list_ptr->max_size = max_size;
    to_do_list_ptr->head = NULL;
    return to_do_list_ptr;
}

void dispose(todo_list_t* todo_list)
{
    node_t* next;
    node_t* p = todo_list->head;
    while (p != NULL) {
        next = p->next;
        free(p->task);
        free(p);
        p = next;
    }
    free(todo_list);
}

bool add_todo(todo_list_t* todo_list, const int32_t priority, const char* task)
{
    node_t* curr;
    node_t* prev = NULL;
    node_t* new_node;
    if (todo_list->cur_size == todo_list->max_size) {
        return false;
    }

    new_node = (node_t*)malloc(sizeof(node_t));
    new_node->priority = priority;
    new_node->task = (char*)malloc((strlen(task) + 1) * sizeof(char));
    strcpy(new_node->task, task);

    curr = todo_list->head;
    while (curr != NULL) {
        if (priority >= curr->priority) {
            break;
        }
        prev = curr;
        curr = curr->next;
    }

    if (prev != NULL) {
        prev->next = new_node;
    } else {
        todo_list->head = new_node;
    }

    if (curr != NULL) {
        new_node->next = curr;
    } else {
        new_node->next = NULL;
    }
    todo_list->cur_size++;
    return true;
}

bool complete_todo(todo_list_t* todo_list)
{
    node_t* p;
    if (todo_list->cur_size == 0) {
        return false;
    }

    p = todo_list->head;
    todo_list->head = p->next;
    free(p);
    todo_list->cur_size--;
    return true;
}

const char* peek_or_null(todo_list_t* todo_list)
{
    if (todo_list->cur_size == 0) {
        return NULL;
    }
    return (const char*)todo_list->head->task;
}

size_t get_count(todo_list_t* todo_list)
{
    return todo_list->cur_size;
}

bool is_empty(todo_list_t* todo_list)
{
    return todo_list->cur_size == 0 ? true : false;
}


