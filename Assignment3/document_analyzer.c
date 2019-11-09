#include "document_analyzer.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static char**** s_document = NULL;

size_t count_paragraphs(const char* content)
{
    size_t count = 0;
    char prev_char;
    const char* ptr = content;

    if (*ptr == '\0') {
        return 0;
    }

    prev_char = *ptr;
    while (*ptr != '\0') {
        if (*ptr == '\n' && prev_char != '\n') {
            count++;
        }
        prev_char = *ptr;
        ptr++;
    }
    if (*(ptr - 1) == '\n') {
        return count;
    }
    return ++count;
}

size_t count_sentence(const char* paragraph)
{
    size_t count = 0;
    const char* ptr = paragraph;

    while (*ptr != '\0') {
        if (*ptr == '!' || *ptr == '.' || *ptr == '?') {
            count++;
        }
        ptr++;
    }
    return count;
}

size_t count_word(const char* sentence)
{
    size_t count = 0;
    const char* ptr = sentence;

    while (!(*ptr == '!' || *ptr == '.' || *ptr == '?')) {
        if (*ptr == ' ') {
            count++;
        }
        ptr++;
    }
    return ++count;
}

void get_next_word(const char* str, size_t start_pos, size_t* new_word_start, size_t* length)
{
    *length = 0;

    while (str[start_pos] == ' ' || str[start_pos] == ',') {
        start_pos++;
    }
    *new_word_start = start_pos;
    while (str[start_pos] != ' ' && str[start_pos] != ',' && str[start_pos] != '.' && str[start_pos] != '!' && str[start_pos] != '?') {
        (*length)++;
        start_pos++;
    }
}

void get_next_sentence(const char* str, size_t start_pos, size_t* new_sentence_start, size_t* length)
{
    *length = 0;

    while (str[start_pos] == ' ') {
        start_pos++;
    }
    *new_sentence_start = start_pos;
    while (str[start_pos] != '.' && str[start_pos] != '!' && str[start_pos] != '?') {
        (*length)++;
        start_pos++;
    }
    (*length)++;
}

int load_document(const char* document)
{
    FILE* fstream;
    content content;
    char* pointer_to_add;
    size_t line_len;
    size_t paragraph_count;
    size_t sentence_count;
    size_t word_count;
    char* paragraph_token;
    size_t i;
    size_t j;
    size_t k;
    size_t sentence_cur;
    size_t sentence_start;
    size_t sentence_length;
    size_t word_cur;
    size_t word_start;
    size_t word_length;
    char line[LINE_LENGTH];

    fstream = fopen(document, "r");
    dispose();
    if (fstream == NULL) {
        return FALSE;
    }

    content.char_arr = (char*)malloc(CONTENT_INIT_LENGTH * sizeof(char));
    content.length = CONTENT_INIT_LENGTH;
    content.remain_buffer = CONTENT_INIT_LENGTH;

    while (fgets(line, LINE_LENGTH, fstream) != NULL) {
        if (content.remain_buffer < LINE_LENGTH) {
            content.remain_buffer += content.length;
            content.length *= 2;
            content.char_arr = realloc(content.char_arr, content.length * sizeof(char));
        }
        pointer_to_add = content.char_arr + (content.length - content.remain_buffer);

        line_len = strlen(line);
        strncpy(pointer_to_add, line, line_len);
        content.remain_buffer -= line_len;
    }

    paragraph_count = count_paragraphs(content.char_arr);
    if (paragraph_count == 0) {
        free(content.char_arr);
        return TRUE;
    }
    s_document = (char****)malloc((paragraph_count + 1) * sizeof(char***));

    paragraph_token = strtok(content.char_arr, "\n");
    i = 0;
    while (paragraph_token != NULL) {
        sentence_count = count_sentence(paragraph_token);
        s_document[i] = (char***)malloc((sentence_count + 1) * sizeof(char**));

        sentence_cur = 0;
        for (j = 0; j < sentence_count; ++j) {
            get_next_sentence(paragraph_token, sentence_cur, &sentence_start, &sentence_length);
            sentence_cur = sentence_start + sentence_length;

            word_count = count_word(&paragraph_token[sentence_start]);
            s_document[i][j] = (char**)malloc((word_count + 1) * sizeof(char*));

            word_cur = sentence_start;
            for (k = 0; k < word_count; ++k) {
                get_next_word(paragraph_token, word_cur, &word_start, &word_length);
                word_cur = word_start + word_length;
                s_document[i][j][k] = (char*)malloc((word_length + 1) * sizeof(char));
                strncpy(s_document[i][j][k], &paragraph_token[word_start], word_length);
                s_document[i][j][k][word_length] = '\0';
            }
            s_document[i][j][k] = NULL;
        }
        s_document[i][j] = NULL;
        paragraph_token = strtok(NULL, "\n");
        i++;
    }
    s_document[paragraph_count] = NULL;
    free(content.char_arr);
    return TRUE;
}

void dispose(void)
{
    size_t i;
    size_t j;
    size_t k;
    if (s_document == NULL) {
        return;
    }
    i = 0;
    while (s_document[i] != NULL) {
        j = 0;
        while (s_document[i][j] != NULL) {
            k = 0;
            while (s_document[i][j][k] != NULL) {
                free(s_document[i][j][k]);
                k++;
            }
            free(s_document[i][j]);
            j++;
        }
        free(s_document[i]);
        i++;
    }
    free(s_document);
    s_document = NULL;
}

size_t get_total_word_count(void)
{
    size_t i;
    size_t j;
    size_t k;
    size_t count = 0;
    if (s_document == NULL) {
        return 0;
    }
    i = 0;
    while (s_document[i] != NULL) {
        j = 0;
        while (s_document[i][j] != NULL) {
            k = 0;
            while (s_document[i][j][k] != NULL) {
                count++;
                k++;
            }
            j++;
        }
        i++;
    }
    return count;
}

size_t get_total_sentence_count(void)
{
    size_t i;
    size_t j;
    size_t count = 0;
    if (s_document == NULL) {
        return 0;
    }
    i = 0;
    while (s_document[i] != NULL) {
        j = 0;
        while (s_document[i][j] != NULL) {
            count++;
            j++;
        }
        i++;
    }
    return count;
}

size_t get_total_paragraph_count(void)
{
    size_t i;
    size_t count = 0;
    if (s_document == NULL) {
        return 0;
    }
    i = 0;
    while (s_document[i] != NULL) {
        count++;
        i++;
    }
    return count;
}

const char*** get_paragraph(const size_t paragraph_index)
{
    if (s_document == NULL) {
        return NULL;
    }
    if (paragraph_index >= get_total_paragraph_count()) {
        return NULL;
    }
    return (const char***)s_document[paragraph_index];
}

size_t get_paragraph_word_count(const char*** paragraph)
{
    size_t i = 0;
    size_t j;
    size_t count = 0;

    while (paragraph[i] != NULL) {
        j = 0;
        while (paragraph[i][j] != NULL) {
            count++;
            j++;
        }
        i++;
    }
    return count;
}

size_t get_paragraph_sentence_count(const char*** paragraph)
{
    size_t i = 0;
    size_t count = 0;
    while (paragraph[i] != NULL) {
        count++;
        i++;
    }
    return count;
}

const char** get_sentence(const size_t paragraph_index, const size_t sentence_index)
{
    const char*** paragraph;
    if (s_document == NULL) {
        return NULL;
    }
    paragraph = get_paragraph(paragraph_index);
    if (paragraph == NULL || get_paragraph_sentence_count(paragraph) <= sentence_index) {
        return NULL;
    }

    return (const char**)s_document[paragraph_index][sentence_index];
}

size_t get_sentence_word_count(const char** sentence)
{
    size_t i = 0;
    size_t count = 0;
    while (sentence[i] != NULL) {
        count++;
        i++;
    }
    return count;
}

int print_as_tree(const char* filename)
{
    size_t i;
    size_t j;
    size_t k;
    size_t paragraph_count;
    FILE* fstream;
    if (s_document == NULL) {
        return FALSE;
    }
    fstream = fopen(filename, "w");
    if (fstream == NULL) {
        return FALSE;
    }

    paragraph_count = get_total_paragraph_count();
    i = 0;
    while (s_document[i] != NULL) {
        fprintf(fstream, "Paragraph %zu:\n", i);
        j = 0;
        while (s_document[i][j] != NULL) {
            fprintf(fstream, "    Sentence %zu:\n", j);
            k = 0;
            while (s_document[i][j][k] != NULL) {
                fprintf(fstream, "        %s\n", s_document[i][j][k]);
                k++;
            }
            j++;
        }
        if (i != paragraph_count - 1) {
            fprintf(fstream, "%s", "\n");
        }
        i++;
    }
    return TRUE;
}

