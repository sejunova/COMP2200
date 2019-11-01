#include <string.h>
#include <stdlib.h>
#include "tokenize.h"

int is_delim(const char c, const char* delim)
{
    const char* p = delim;
    while (*p != '\0') {
        if (*p == c) {
            return TRUE;
        }
        p++;
    }
    return FALSE;
}

size_t get_token_array_size(const char* str, const char* delim)
{
    size_t delim_count = 0;
    size_t start = 0;
    size_t end = 0;
    size_t str_len;

    int i = 0;
    while (str[i] != '\0') {
        if (is_delim(str[i], delim)) {
            str_len = end - start + 1;
            if (str_len > 1) {
                delim_count++;
            }
            start = end + 1;
        }
        end++;
        i++;
    }
    if (!is_delim(str[i - 1], delim)) {
        delim_count++;
    }
    return delim_count;
}

char** tokenize_malloc(const char* str, const char* delim)
{
    size_t i = 0;
    size_t j = 0;
    size_t str_len;
    size_t start = 0;
    size_t end = 0;
    const size_t TOKEN_ARR_LENGTH = get_token_array_size(str, delim);
    char** token_arr = (char**)malloc((TOKEN_ARR_LENGTH + 1) * sizeof(char*));

    while (str[j] != '\0') {
        if (is_delim(str[j], delim)) {
            str_len = end - start + 1;
            if (str_len > 1) {
                token_arr[i] = (char *) malloc(str_len * sizeof(char));
                strncpy(token_arr[i], &str[start], str_len);
                token_arr[i][str_len - 1] = '\0';
                i++;
            }
            start = end + 1;
        }
        end++;
        j++;
    }
    if (!is_delim(str[j - 1], delim)) {
        str_len = end - start + 1;
        if (str_len > 1) {
            token_arr[i] = (char *) malloc(str_len * sizeof(char));
            strncpy(token_arr[i], &str[start], str_len);
            token_arr[i][str_len - 1] = '\0';
            i++;
        }
    }
    token_arr[i] = NULL;

    return token_arr;
}

