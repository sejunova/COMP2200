#include <stdio.h>
#include <limits.h>
#include "array.h"

int get_index_of(const int numbers[], const size_t element_count, const int num)
{
    size_t i;
    for (i = 0; i < element_count; i++) {
        if (numbers[i] == INT_MIN) {
            break;
        }
        if (numbers[i] == num) {
            return i;
        }
    }
    return -1;
}

int get_last_index_of(const int numbers[], const size_t element_count, const int num)
{
    size_t i;
    int last_index = -1;
    for (i = 0; i < element_count; i++) {
        if (numbers[i] == INT_MIN) {
            break;
        }
        if (numbers[i] == num) {
            last_index = i;
        }
    }
    return last_index;
}

int get_max_index(const int numbers[], const size_t element_count)
{
    size_t i;
    int max_value = INT_MIN;
    int max_value_index = -1;

    for (i = 0; i < element_count; i++) {
        if (numbers[i] == INT_MIN) {
            break;
        }
        if (numbers[i] > max_value) {
            max_value = numbers[i];
            max_value_index = i;
        }
    }
    return max_value_index;
}

int get_min_index(const int numbers[], const size_t element_count)
{
    size_t i;
    int min_value = INT_MAX;
    int min_value_index = -1;

    for (i = 0; i < element_count; i++) {
        if (numbers[i] == INT_MIN) {
            break;
        }
        if (numbers[i] < min_value) {
            min_value = numbers[i];
            min_value_index = i;
        }
    }
    return min_value_index;
}

int is_all_positive(const int numbers[], const size_t element_count)
{
    size_t i;
    int is_positive = TRUE;

    if (element_count == 0) {
        return FALSE;
    }
    for (i = 0; i < element_count; i++) {
        if (numbers[i] == INT_MIN) {
            break;
        }
        if (numbers[i] <= 0) {
            is_positive = FALSE;
            break;
        }
    }
    return is_positive;
}

int has_even(const int numbers[], const size_t element_count)
{
    int has_even = FALSE;
    size_t i;
    for (i = 0; i < element_count; i++) {
        if (numbers[i] == INT_MIN) {
            break;
        }
        if (numbers[i] % 2 == 0) {
            has_even = TRUE;
            break;
        }
    }
    return has_even;
}

int insert(int numbers[], const size_t element_count, const int num, const size_t pos)
{
    size_t i;
    if (pos > element_count) {
        return FALSE;
    }

    for (i = element_count; i >= pos; i--) {
        numbers[i + 1] = numbers[i];

        /* prevent buffer overflow */
        if (i == 0) {
            break;
        }
    }
    numbers[pos] = num;
    return TRUE;
}

int remove_at(int numbers[], const size_t element_count, const size_t index)
{
    size_t i;
    if (index >= element_count) {
        return FALSE;
    }

    for (i = index; i < element_count - 1; i++) {
        numbers[i] = numbers[i + 1];
    }
    numbers[i] = INT_MIN;
    return TRUE;
}
