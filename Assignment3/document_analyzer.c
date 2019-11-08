#include "document_analyzer.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static size_t s_total_word_count;
static size_t s_total_sentence_count;
static size_t s_total_paragraph_count;

static char**** s_document;
static sub_paragraph_count* sub_paragraph_counts;


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
    size_t found_word = FALSE;
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
    size_t found_word = FALSE;
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

    fstream = fopen(document, "r");
    if (fstream == NULL) {
        s_total_paragraph_count = 0;
        s_total_sentence_count = 0;
        s_total_word_count = 0;
        return FALSE;
    }

    content.char_arr = (char*)malloc(CONTENT_INIT_LENGTH * sizeof(char));
    content.length = CONTENT_INIT_LENGTH;
    content.remain_buffer = CONTENT_INIT_LENGTH;

    char line[LINE_LENGTH];
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

    s_total_paragraph_count = count_paragraphs(content.char_arr);
    sub_paragraph_counts = (sub_paragraph_count*)malloc(sizeof(sub_paragraph_count) * s_total_paragraph_count);
    s_document = (char****)malloc(s_total_paragraph_count * sizeof(char***));

    paragraph_token = strtok(content.char_arr, "\n");
    i = 0;
    while (paragraph_token != NULL) {
        sentence_count = count_sentence(paragraph_token);
        sub_paragraph_counts[i].sentence_count = sentence_count;
        sub_paragraph_counts[i].sentence_word_counts = (size_t*)malloc(sentence_count * sizeof(size_t));
        s_document[i] = (char***)malloc((sentence_count + 1) * sizeof(char**));
        s_total_sentence_count += sentence_count;

        sentence_cur = 0;
        for (j = 0; j < sentence_count; ++j) {
            get_next_sentence(paragraph_token, sentence_cur, &sentence_start, &sentence_length);
            sentence_cur = sentence_start + sentence_length;

            word_count = count_word(&paragraph_token[sentence_start]);
            sub_paragraph_counts[i].sentence_word_counts[j] = word_count;
            s_total_word_count += word_count;
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
    free(content.char_arr);
}

void dispose(void)
{
    size_t i;
    size_t j;
    size_t k;
    for (i = 0; i < s_total_paragraph_count; ++i) {
        j = 0;
        while (TRUE) {
            if (s_document[i][j] == NULL) {
                free(s_document[i][j]);
                break;
            }
            k = 0;
            while (s_document[i][j][k] != NULL) {
                if (s_document[i][j][k] != NULL) {
                    free(s_document[i][j][k]);
                    break;
                }
                free(s_document[i][j][k]);
                k++;
            }
            j++;
        }
        free(s_document[i]);
    }
}

size_t get_total_word_count(void)
{
    return s_total_word_count;
}

size_t get_total_sentence_count(void)
{
    return s_total_sentence_count;
}

size_t get_total_paragraph_count(void)
{
    return s_total_paragraph_count;
}

const char*** get_paragraph(const size_t paragraph_index)
{
    return (const char***)s_document[paragraph_index];
}

size_t get_paragraph_word_count(const char*** paragraph)
{

}






int print_as_tree(const char* filename)
{
    // 파일스트림 열다가 에러나면 FALSE 리턴하자
    size_t i;
    size_t j;
    size_t k;
    for (i = 0; i < s_total_paragraph_count; ++i) {
        printf("Paragraph %d:\n", i);
        j = 0;
        while (s_document[i][j] != NULL) {
            printf("    Sentence %d:\n", j);
            k = 0;
            while (s_document[i][j][k] != NULL) {
                printf("        %s\n", s_document[i][j][k]);
                k++;
            }
            j++;
        }
    }
    return TRUE;
}





