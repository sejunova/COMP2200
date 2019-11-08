#include "document_analyzer.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static size_t s_total_word_count;
static size_t s_total_sentence_count;
static size_t s_total_paragraph_count;

static char**** s_document;

typedef struct {
    size_t remain_buffer;
    char* char_arr;
    size_t length;
} content;

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

    while (*ptr != '\0') {
        if (*ptr == ' ') {
            count++;
        }
        ptr++;
    }
    return count;
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
    char* sentence_token;
    char* word_token;
    size_t i;
    size_t j;
    size_t k;

    fstream = fopen(document, "r");
    if (fstream == NULL) {
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
    s_document = (char****)malloc(s_total_paragraph_count * sizeof(char***));

    paragraph_token = strtok(content.char_arr, "\n");
    i = 0;
    while (paragraph_token != NULL) {
        j = 0;
        sentence_count = count_sentence(paragraph_token);
        s_document[i] = (char***)malloc((sentence_count + 1) * sizeof(char**));
        s_total_sentence_count += sentence_count;

        sentence_token = strtok(paragraph_token, "!?.");
        while (sentence_token != NULL) {
            k = 0;
            word_count = count_word(sentence_token);
            s_total_word_count += word_count;
            s_document[i][j] = (char**)malloc((word_count + 1) * sizeof(char*));

            word_token = strtok(sentence_token, ", ");
            while (word_token != NULL) {
                s_document[i][j][k] = (char*)malloc((strlen(word_token) + 1) * sizeof(char));
                strcpy(s_document[i][j][k], word_token);
                word_token = strtok(NULL, ", ");
                k++;
            }
            s_document[i][j][k] = NULL;
            sentence_token = strtok(NULL, "!?.");
            j++;
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

int print_as_tree(const char* filename)
{
    // 파일스트림 열다가 에러나면 FALSE 리턴하자
    size_t i;
    size_t j;
    size_t k;
    for (i = 0; i < s_total_paragraph_count; ++i) {
        printf("Paragraph %d:", i);
        j = 0;
        while (s_document[i][j] != NULL) {
            printf("    Sentence %d:", j);
            k = 0;
            while (s_document[i][j][k] != NULL) {
                printf("        %s", s_document[i][j][k]);
                k++;
            }
            j++;
        }
    }
    return TRUE;
}



