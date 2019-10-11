#include "character_deserializer.h"

int deserialize_version_1(FILE* file, character_v3_t* out_character)
{
    char line[1024];
    char* tok;
    int ret = -1;

    rewind(file);
    if (fgets(line, LINE_SIZE, file) != NULL) {
        tok = strtok(line,":,");
        while (tok != NULL)
        {
            tok = strtok (NULL, ":,");

        }
        ret = 0;
    }
    return ret;
}

int deserialize_version_2(FILE* file, character_v3_t* out_character)
{

}

int deserialize_version_3(FILE* file, character_v3_t* out_character)
{

}

int get_character(const char* filename, character_v3_t* out_character)
{
    FILE* file;
    char line[LINE_SIZE];
    int ret = -1;

    file = fopen(filename, "r");

    if (fgets(line, LINE_SIZE, file) != NULL) {
        if (strstr(line, ":") != NULL) {
            ret = deserialize_version_1(file, out_character);
        } else if (strstr(line, "|") != NULL) {
            ret = deserialize_version_1(file, out_character);
        } else {
            ret = deserialize_version_2(file, out_character);
        }
    }
    fclose(file);
    return ret;
}

