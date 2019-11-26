#include "data_store.h"
#include "string.h"

#if defined(RELEASE)
void encrypt_email(char* dest, const char* org)
{
    int i = (int)(strchr(org, '@') - org);
    strcpy(dest, org);
    size_t dest_len = strlen(org);
    if (i == 1) {
        dest[0] = '*';
    } else if (i == 2) {
        dest[1] = '*';
    } else {
        memset(dest + 1, '*', i - 2);
    }
}

void encrypt_password(char* dest, const char* org)
{
    size_t dest_len = strlen(org);
    memset(dest, '*', dest_len);
    if (dest_len == 2) {
        dest[0] = org[0];
    } else if (dest_len >= 3) {
        dest[0] = org[0];
        dest[dest_len - 1] = org[dest_len - 1];
    }
    dest[dest_len] = '\0';
}
#endif

user_t* get_user_by_id_or_null(user_t** users_or_null, size_t id)
{
    user_t** p = users_or_null;
    if (p == NULL || *p == NULL) {
        return NULL;
    }

    while (*p != NULL) {
        if ((*p)->id == id) {
            return *p;
        }
        p++;
    }
    return NULL;
}

user_t* get_user_by_username_or_null(user_t** users_or_null, const char* username)
{
    user_t** p = users_or_null;
    if (p == NULL || *p == NULL) {
        return NULL;
    }

    while (*p != NULL) {
        if (strcmp((*p)->username, username) == 0) {
            return *p;
        }
        p++;
    }
    return NULL;
}

bool update_email(user_t** users_or_null, size_t id, const char* email)
{
    user_t** p = users_or_null;
    FILE* fstream;
    char line[200];
    if (p == NULL || *p == NULL) {
        return false;
    }

    while (*p != NULL) {
        if ((*p)->id == id) {
            fstream = fopen("log.txt", "a");
#if defined(RELEASE)
            char email_encrypted[51];
            char new_email_encrypted[51];
            encrypt_email(email_encrypted, (*p)->email);
            encrypt_email(new_email_encrypted, email);
            sprintf(line, "TRACE: User %zd updated email from \"%s\" to \"%s\"\n", (*p)->id, email_encrypted, new_email_encrypted);
#else
            sprintf(line, "TRACE: User %zd updated email from \"%s\" to \"%s\"\n", (*p)->id, (*p)->email, email);
#endif
            fwrite(line, strlen(line), 1, fstream);
            fclose(fstream);
            strcpy((*p)->email, email);
            return true;
        }
        p++;
    }
    return false;
}

bool update_password(user_t** users_or_null, size_t id, const char* password)
{
    user_t** p = users_or_null;
    FILE* fstream;
    char line[200];
    if (p == NULL || *p == NULL) {
        return false;
    }

    while (*p != NULL) {
        if ((*p)->id == id) {
            fstream = fopen("log.txt", "a");
#if defined(RELEASE)
            char password_encrypted[51];
            char new_password_encrypted[51];
            encrypt_password(password_encrypted, (*p)->password);
            encrypt_password(new_password_encrypted, password);
            sprintf(line, "TRACE: User %zd updated password from \"%s\" to \"%s\"\n", (*p)->id, password_encrypted, new_password_encrypted);
#else
            sprintf(line, "TRACE: User %zd updated password from \"%s\" to \"%s\"\n", (*p)->id, (*p)->password, password);
#endif
            fwrite(line, strlen(line), 1, fstream);
            fclose(fstream);
            strcpy((*p)->password, password);
            return true;
        }
        p++;
    }
    return false;
}

