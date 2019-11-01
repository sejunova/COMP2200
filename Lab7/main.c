#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "tokenize.h"

int main(void)
{
    const char* str = " good time";
    char** tokens = tokenize_malloc(str, " ?!.");
    char** tt = tokens;

    assert(strcmp(*tt++, "good") == 0);
    assert(strcmp(*tt++, "time") == 0);
    assert(*tt == NULL);

    tt = tokens;
    while (*tt != NULL) {
        free(*tt++);
    }

    free(tokens);

    return 0;
}

