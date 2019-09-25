#include <stdio.h>
#include "my_string.h"

int get_strlen_until_c(const char* str, char c)
{
    int length = 0;
    while (*str++ != c) {
        length++;
    }
    return length;
}

int strcmp_n(const char* str1, const char* str2, int n)
{
    int i = 0;
    while (i < n) {
        if (*str1 != *str2) {
            return *str1 - *str2;
        }
        str1++;
        str2++;
        i++;
    }
    return 0;
}

void reverse_n(char* str, int n)
{
    int i;
    char temp;
    for (i = 0; i < n / 2; ++i) {
        temp = str[i];
        str[i] = str[n - i - 1];
        str[n - i - 1] = temp;
    }
}

int contain_char(const char* str, const char c)
{
    while (*str != '\0') {
        if (*str == c) {
            return TRUE;
        }
        str++;
    }
    return FALSE;
}

void reverse(char* str)
{
    int str_len;

    if (str == NULL) {
        return;
    }
    str_len = get_strlen_until_c(str, '\0');
    reverse_n(str, str_len);
}

int index_of(const char* str, const char* word)
{
    int i;
    int str_len;
    int word_len;
    if (str == NULL || word == NULL) {
        return -1;
    }

    str_len = get_strlen_until_c(str, '\0');
    word_len = get_strlen_until_c(word, '\0');

    if (str_len == 0 || word_len == 0 || word_len > str_len) {
        return -1;
    }

    for (i = 0; i < str_len - word_len + 1; ++i) {
        if (strcmp_n(&str[i], word, word_len) == 0) {
            return i;
        }
    }
    return -1;
}

void reverse_by_words(char* str)
{
    int word_len = 0;
    char* p_word_start = NULL;

    if (str == NULL) {
        return;
    }

    while (*str != '\0') {
        if (p_word_start == NULL && *str != ' ') {
            p_word_start = str;
        }
        if (*str == ' ') {
            if (word_len >= 2) {
                reverse_n(p_word_start, word_len);
            }
            p_word_start = NULL;
            word_len = 0;
        } else {
            word_len++;
        }
        str++;
    }

    if (p_word_start != NULL && word_len >= 2) {
        reverse_n(p_word_start, word_len);
    }
}

char* tokenize(char* str, const char* delims)
{
    char* p;
    char* token_start;
    static char* s_token_start;

    if (str == NULL && s_token_start == NULL) {
        return NULL;
    }
    if (str != NULL) {
        s_token_start = str;
    }

    for (p = s_token_start; *p != '\0' && contain_char(delims, *p) == TRUE; ++p) {
        *p = '\0';
    }

    /* not found a token */
    if (*p == '\0') {
        return p;
    }

    s_token_start = p;
    /* found a token */
    for (p = s_token_start; *p != '\0' && contain_char(delims, *p) == FALSE; ++p) {
    }
    *p = '\0';

    token_start = s_token_start;
    s_token_start = p + 1;
    return token_start;
}

char* reverse_tokenize(char* str, const char* delims)
{
    char* token_start;
    token_start = tokenize(str, delims);
    reverse(token_start);
    return token_start;
}

