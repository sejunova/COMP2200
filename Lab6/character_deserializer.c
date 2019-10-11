#include "character_deserializer.h"

void id_to_name(const char *id, char *out)
{
    unsigned int i = 7;
    const char *ptr = id;
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

unsigned int my_atoi(const char *tok)
{
    int integer = 0;
    const char *ptr = tok;

    while (*ptr != '\0') {
        integer *= 10;
        integer += (*ptr - '0');
        ptr++;
    }
    return integer;
}

void deserialize_version_1(FILE *file, character_v3_t *out_character)
{
    char line[1024];
    char *tok;
    int bAttribute = TRUE;
    char attribute[6];

    rewind(file);
    if (fgets(line, LINE_SIZE, file) != NULL) {
        tok = strtok(line, ":,\n\r");
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
                    out_character->evasion = my_atoi(tok) / 2;
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
                    out_character->mana = my_atoi(tok);
                }
                bAttribute = TRUE;
            }
            tok = strtok(NULL, ":,\n\r");
        }
    }
}

void deserialize_version_2(FILE *file, character_v3_t *out_character)
{
    unsigned int i = 0;
    char *tok;
    char line[1024];

    fgets(line, LINE_SIZE, file);
    tok = strtok(line, ",\n\r");
    while (tok != NULL) {
        switch (i++) {
        case 0:
            strncpy(out_character->name, tok, 50);
            out_character->name[50] = '\0';
            break;
        case 1:
            out_character->level = my_atoi(tok);
            out_character->leadership = my_atoi(tok) / 10;
            break;
        case 2:
            out_character->strength = my_atoi(tok);
            break;
        case 3:
            out_character->dexterity = my_atoi(tok);
            break;
        case 4:
            out_character->intelligence = my_atoi(tok);
            break;
        case 5:
            out_character->armour = my_atoi(tok);
            break;
        case 6:
            out_character->evasion = my_atoi(tok);
            break;
        case 7:
            out_character->elemental_resistance.cold = my_atoi(tok) / 3;
            out_character->elemental_resistance.fire = my_atoi(tok) / 3;
            out_character->elemental_resistance.lightning = my_atoi(tok) / 3;
            break;
        case 8:
            out_character->health = my_atoi(tok);
            break;
        case 9:
            out_character->mana = my_atoi(tok);
            break;
        default:
            break;
        }
        tok = strtok(NULL, ",\n\r");
    }
    out_character->minion_count = 0;
}

void deserialize_version_3(FILE *file, character_v3_t *out_character)
{
    unsigned int i = 0;
    unsigned int j = 0;
    char *tok;
    char line[1024];

    fgets(line, LINE_SIZE, file);
    tok = strtok(line, " |\n\r");
    while (tok != NULL) {
        switch (i++) {
        case 0:
            strncpy(out_character->name, tok, 50);
            out_character->name[50] = '\0';
            break;
        case 1:
            out_character->level = my_atoi(tok);
            break;
        case 2:
            out_character->health = my_atoi(tok);
            break;
        case 3:
            out_character->mana = my_atoi(tok);
            break;
        case 4:
            out_character->strength = my_atoi(tok);
            break;
        case 5:
            out_character->dexterity = my_atoi(tok);
            break;
        case 6:
            out_character->intelligence = my_atoi(tok);
            break;
        case 7:
            out_character->armour = my_atoi(tok);
            break;
        case 8:
            out_character->evasion = my_atoi(tok);
            break;
        case 9:
            out_character->elemental_resistance.fire = my_atoi(tok);
            break;
        case 10:
            out_character->elemental_resistance.cold = my_atoi(tok);
            break;
        case 11:
            out_character->elemental_resistance.lightning = my_atoi(tok);
            break;
        case 12:
            out_character->leadership = my_atoi(tok);
            break;
        case 13:
            out_character->minion_count = my_atoi(tok);
            break;
        default:
            break;
        }
        tok = strtok(NULL, " |\n\r");
    }

    i = 0;
    if (fgets(line, LINE_SIZE, file) != NULL) {
        while (fgets(line, LINE_SIZE, file) != NULL) {
            tok = strtok(line, " |\n\r");
            j = 0;
            while (tok != NULL) {
                switch (j++) {
                case 0:
                    strncpy(out_character->minions[i].name, tok, 50);
                    out_character->minions[i].name[50] = '\0';
                    break;
                case 1:
                    out_character->minions[i].health = my_atoi(tok);
                    break;
                case 2:
                    out_character->minions[i].strength = my_atoi(tok);
                    break;
                case 3:
                    out_character->minions[i].defence = my_atoi(tok);
                    break;
                case 4:
                    out_character->minions[i].strength = my_atoi(tok);
                    break;
                default:
                    break;
                }
                tok = strtok(NULL, " |\n\r");
            }
            i++;
        }
    }
}

int get_character(const char *filename, character_v3_t *out_character)
{
    FILE *file;
    char line[LINE_SIZE];
    int version = 0;

    file = fopen(filename, "rb");

    if (fgets(line, LINE_SIZE, file) != NULL) {
        if (strstr(line, ":") != NULL) {
            deserialize_version_1(file, out_character);
            version = 1;
        } else if (strstr(line, "|") != NULL) {
            deserialize_version_3(file, out_character);
            version = 3;
        } else {
            deserialize_version_2(file, out_character);
            version = 2;
        }
    }
    fclose(file);
    return version;
}

