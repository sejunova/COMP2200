#ifndef USER_H
#define USER_H

#include <stdio.h>

typedef struct {
    size_t id;
    char username[51];
    char email[51];
    char password[51];
} user_t;
#endif /* USER_H */
