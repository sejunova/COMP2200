#include "character_deserializer.h"

void id_to_name(const char* id, char* out)
{
    unsigned int i = 7;
    const char* ptr = id;
    out[0] = 'p';
    out[1] = 'l';
    out[2] = 'a';
    out[3] = 'y';
    out[4] = 'e';
    out[5] = 'r';
    out[6] = '_';

    while (i < 50 || *ptr != '\0') {
        out[i] = *ptr;
        i++;
        ptr++;
    }
    out[i] = *ptr;
}

unsigned int my_atoi(const char *tok) {
    int integer = 0;
    const char *ptr = tok;

    while (*ptr != '\0') {
        integer *= 10;
        integer += (*ptr - '0');
        ptr++;
    }
    return integer;
}

void deserialize_version_1(FILE *file, character_v3_t *out_character) {
    char line[1024];
    char *tok;
    int bAttribute = TRUE;
    char attribute[6];

    rewind(file);
    if (fgets(line, LINE_SIZE, file) != NULL) {
        tok = strtok(line, ":,");
        while (tok != NULL) {
            if (bAttribute == TRUE) {
                strcpy(attribute, tok);
                bAttribute = FALSE;
            } else {
                if (strcmp(attribute, "lvl") == 0) {
                    out_character->level = my_atoi(tok);
                    out_character->leadership = my_atoi(tok) / 10;
                } else if (strcmp(attribute, "intel") == 0) {
                    out_character->intelligence = my_atoi(tok);
                } else if (strcmp(attribute, "str") == 0) {
                    out_character->strength = my_atoi(tok);
                } else if (strcmp(attribute, "dex") == 0) {
                    out_character->dexterity = my_atoi(tok);
                    out_character->elemental_resistance.fire = my_atoi(tok) / 4 / 3;
                    out_character->elemental_resistance.cold = my_atoi(tok) / 4 / 3;
                    out_character->elemental_resistance.lightning = my_atoi(tok) / 4 / 3;
                } else if (strcmp(attribute, "def") == 0) {
                    out_character->armour = my_atoi(tok);
                } else if (strcmp(attribute, "id") == 0) {
                    id_to_name(tok, out_character->name);
                } else if (strcmp(attribute, "hp") == 0) {
                    out_character->health = my_atoi(tok);
                } else if (strcmp(attribute, "mp") == 0) {
                    out_character->health = my_atoi(tok);
                }
            }
            tok = strtok(NULL, ":,\n");
        }
    }
}

void deserialize_version_2(FILE *file, character_v3_t *out_character) {

}

void deserialize_version_3(FILE *file, character_v3_t *out_character) {

}

int get_character(const char *filename, character_v3_t *out_character) {
    FILE *file;
    char line[LINE_SIZE];
    int version = 0;

    file = fopen(filename, "r");

    if (fgets(line, LINE_SIZE, file) != NULL) {
        if (strstr(line, ":") != NULL) {
            deserialize_version_1(file, out_character);
            version = 1;
        } else if (strstr(line, "|") != NULL) {
            deserialize_version_1(file, out_character);
            version = 2;
        } else {
            deserialize_version_2(file, out_character);
            version = 3;
        }
    }
    fclose(file);
    return version;
}

