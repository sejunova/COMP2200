#ifndef TRANSLATE_H
#define TRANSLATE_H

#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define TRUE (1)
#define FALSE (0)

#define BUFFER_SIZE (512)

typedef enum error_code {
    WRONG_ARGUMENTS_NUMBER = 1,
    INVALID_FLAG = 2,
    INVALID_FORMAT = 3,
    ARGUMENT_TOO_LONG = 4,
    INVALID_RANGE = 5
} error_code_t;

int translate(int argc, const char** argv);

#endif /* TRANSLATE_H */

