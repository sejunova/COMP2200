#include <stdlib.h>
#include <string.h>
#include "hashmap.h"

hashmap_t* init_hashmap_malloc(size_t length, int (*p_hash_func)(const char* key))
{
    size_t i;
    hashmap_t* hashmap = (hashmap_t*)malloc(sizeof(hashmap_t));
    hashmap->length = length;
    hashmap->hash_func = p_hash_func;
    hashmap->plist = (node_t**)malloc(length * sizeof(node_t*));
    for (i = 0; i < length; ++i) {
        hashmap->plist[i] = NULL;
    }
    return hashmap;
}

int add_key(hashmap_t* hashmap, const char* key, const int value)
{
    node_t* p;
    int hash_value = abs(hashmap->hash_func(key));
    int index = hash_value % (int)hashmap->length;

    if (hashmap->plist[index] == NULL) {
        hashmap->plist[index] = (node_t*)malloc(sizeof(node_t));
        p = hashmap->plist[index];
        p->key = (char*)malloc(strlen(key) * sizeof(char));
        strcpy(p->key, key);
        p->value = value;
        p->next = NULL;
        return TRUE;
    }

    p = hashmap->plist[index];
    while (TRUE) {
        if (strcmp(p->key, key) == 0) {
            return FALSE;
        }
        if (p->next == NULL) {
            break;
        }
        p = p->next;
    }
    p->next = (node_t*)malloc(sizeof(node_t));
    p = p->next;
    p->key = (char*)malloc(strlen(key) * sizeof(char));
    strcpy(p->key, key);
    p->value = value;
    p->next = NULL;
    return TRUE;
}

int get_value(hashmap_t* hashmap, const char* key)
{
    node_t* p;
    int hash_value = abs(hashmap->hash_func(key));
    int index = (int)(hash_value % hashmap->length);

    p = hashmap->plist[index];
    while (p != NULL) {
        if (strcmp(p->key, key) == 0) {
            return p->value;
        }
        p = p->next;
    }
    return -1;
}

int update_value(hashmap_t* hashmap, const char* key, int value)
{
    node_t* p;
    int hash_value = abs(hashmap->hash_func(key));
    int index = (int)(hash_value % hashmap->length);

    p = hashmap->plist[index];
    while (p != NULL) {
        if (strcmp(p->key, key) == 0) {
            p->value = value;
            return TRUE;
        }
        p = p->next;
    }
    return FALSE;
}

int remove_key(hashmap_t* hashmap, const char* key)
{
    node_t* p;
    node_t* prev = NULL;
    int hash_value = abs(hashmap->hash_func(key));
    int index = (int)(hash_value % hashmap->length);
    p = hashmap->plist[index];
    if (p == NULL) {
        return FALSE;
    }

    while (p != NULL) {
        if (strcmp(p->key, key) == 0) {
            if (prev == NULL) {
                hashmap->plist[index] = p->next;
            } else {
                prev->next = p->next;
            }
            free(p->key);
            free(p);
            return TRUE;
        }
        prev = p;
        p = p->next;
    }
    return FALSE;
}

void destroy(hashmap_t* hashmap)
{
    size_t i;
    node_t* p;
    node_t* next;
    for (i = 0; i < hashmap->length; ++i) {
        p = hashmap->plist[i];
        while (p != NULL) {
            next = p->next;
            free(p->key);
            free(p);
            p = next;
        }
    }
    free(hashmap);
}

