#ifndef DOCUMENT_ANALYZER_H
#define DOCUMENT_ANALYZER_H

#include <stdio.h>

#define TRUE (1)
#define FALSE (0)

#define CONTENT_INIT_LENGTH (4096)
#define LINE_LENGTH (1024)

typedef struct {
    size_t sentence_count;
    size_t* sentence_word_counts;
} sub_paragraph_count;

typedef struct {
    size_t remain_buffer;
    char* char_arr;
    size_t length;
} content;

int load_document(const char* document);

void dispose(void);

size_t get_total_word_count(void);

size_t get_total_sentence_count(void);

size_t get_total_paragraph_count(void);

const char*** get_paragraph(const size_t paragraph_index);

size_t get_paragraph_word_count(const char*** paragraph);

size_t get_paragraph_sentence_count(const char*** paragraph);

const char** get_sentence(const size_t paragraph_index, const size_t sentence_index);

size_t get_sentence_word_count(const char** sentence);

int print_as_tree(const char* filename);

#endif /* DOCUMENT_ANALYZER_H */