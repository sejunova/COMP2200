#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "todo_list.h"

int main(void)
{
    todo_list_t* todo_list = init_todo_list_malloc(9);

    assert(is_empty(todo_list));

    assert(add_todo(todo_list, 10, "A"));
    assert(add_todo(todo_list, 23, "B"));
    assert(add_todo(todo_list, 10, "C"));
    assert(add_todo(todo_list, 23, "D"));
    assert(add_todo(todo_list, 10, "E"));
    assert(add_todo(todo_list, 59, "F"));
    assert(add_todo(todo_list, 60, "G"));
    assert(add_todo(todo_list, 55, "H"));
    assert(add_todo(todo_list, 2, "I"));

    assert(get_count(todo_list) == 9);
    assert(!is_empty(todo_list));
	print_task(todo_list);


    dispose(todo_list);

    return 0;
}

