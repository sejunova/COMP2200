#include "translate.h"

int translate(int argc, const char** argv)
{
    error_code_t wrong_arguemnt_number = WRONG_ARGUMENTS_NUMBER;
    error_code_t invalid_flag = INVALID_FLAG;
    error_code_t invalid_format = INVALID_FORMAT;
    error_code_t argument_too_long = ARGUMENT_TOO_LONG;
    error_code_t invalid_range = INVALID_RANGE;

    int i;
    int c;
    char temp_char;
    char prev_char;
    char next_char;

    char set1_translated[BUFFER_SIZE];
    char set2_translated[BUFFER_SIZE];

    const char* p_set1_start;
    const char* p_set2_start;

    const char* p_set1;
    const char* p_set2;

    int set1_count = 0;
    int set2_count = 0;
    const char* last_range_end_index = NULL;

    /* 인자 수가 안 맞는 경우 오류 반환 */
    if (!(argc == 3 || (argc == 4 && strcmp(argv[1], "-i") == 0))) {
        return wrong_arguemnt_number;
    }
    if (argc == 4 && strcmp(argv[1], "-i") != 0) {
        return invalid_flag;
    }

    if (argc == 3) {
        p_set1_start = argv[1];
        p_set2_start = argv[2];
        p_set1 = argv[1];
        p_set2 = argv[2];
    } else {
        p_set1_start = argv[2];
        p_set2_start = argv[3];
        p_set1 = argv[2];
        p_set2 = argv[3];
    }

    /* SET1 PART*/
    while (*p_set1 != '\0') {
        if (*p_set1 == 39 || *p_set1 == 34) {
            p_set1++;
            continue;
        }
        if (set1_count == BUFFER_SIZE - 1) {
            return argument_too_long;
        }

        if (*p_set1 == 92) {
            p_set1++;
            if (*p_set1 == 92) {
                set1_translated[set1_count] = '\\';
            } else if (*p_set1 == 'a') {
                set1_translated[set1_count] = '\a';
            } else if (*p_set1 == 'b') {
                set1_translated[set1_count] = '\b';
            } else if (*p_set1 == 'f') {
                set1_translated[set1_count] = '\f';
            } else if (*p_set1 == 'n') {
                set1_translated[set1_count] = '\n';
            } else if (*p_set1 == 'r') {
                set1_translated[set1_count] = '\r';
            } else if (*p_set1 == 't') {
                set1_translated[set1_count] = '\t';
            } else if (*p_set1 == 'v') {
                set1_translated[set1_count] = '\v';
            } else if (*p_set1 == 39) {
                set1_translated[set1_count] = '\'';
            } else if (*p_set1 == 34) {
                set1_translated[set1_count] = '\"';
            } else {
                return invalid_format;
            }
        } else if (*p_set1 == '-') {
            /* 첫 번째 문자 혹은 마지막 문자가 - 인 경우 혹은 이전 문자가 range의 마지막인 경우 */
            if (p_set1 == p_set1_start || *(p_set1 + 1) == '\0' ||
                (last_range_end_index != NULL && p_set1 - 1 == last_range_end_index)) {
                set1_translated[set1_count] = '-';
            } else {
                if (*(p_set1 + 1) == 92) {
                    p_set1++;
                    if (*(p_set1 + 1) == 92) {
                        next_char = '\\';
                    } else if (*(p_set1 + 1) == 'a') {
                        next_char = '\a';
                    } else if (*(p_set1 + 1) == 'b') {
                        next_char = '\b';
                    } else if (*(p_set1 + 1) == 'f') {
                        next_char = '\f';
                    } else if (*(p_set1 + 1) == 'n') {
                        next_char = '\n';
                    } else if (*(p_set1 + 1) == 'r') {
                        next_char = '\r';
                    } else if (*(p_set1 + 1) == 't') {
                        next_char = '\t';
                    } else if (*(p_set1 + 1) == 'v') {
                        next_char = '\v';
                    } else if (*(p_set1 + 1) == 39) {
                        next_char = '\'';
                    } else if (*(p_set1 + 1) == 34) {
                        next_char = '\"';
                    } else {
                        return invalid_format;
                    }
                } else {
                    next_char = *(p_set1 + 1);
                }
                prev_char = set1_translated[set1_count - 1];
                if (prev_char > next_char) {
                    return invalid_range;
                }
                for (i = 0; (char)(set1_translated[set1_count - 1]) < next_char; ++i) {
                    if (set1_count == BUFFER_SIZE - 1) {
                        return argument_too_long;
                    }
                    set1_translated[set1_count] = (char)(set1_translated[set1_count - 1] + 1);
                    set1_count++;
                }
                p_set1++;
                set1_count--;
                last_range_end_index = p_set1;
            }
        } else {
            set1_translated[set1_count] = *p_set1;
        }
        p_set1++;
        set1_count++;
    }
    set1_translated[set1_count] = '\0';

    last_range_end_index = NULL;
    /* SET2 PART*/
    while (*p_set2 != '\0') {
        if (*p_set2 == 39 || *p_set2 == 34) {
            p_set2++;
            continue;
        }
        if (set2_count == BUFFER_SIZE - 1) {
            return argument_too_long;
        }

        if (*p_set2 == 92) {
            p_set2++;
            if (*p_set2 == 92) {
                set2_translated[set2_count] = '\\';
            } else if (*p_set2 == 'a') {
                set2_translated[set2_count] = '\a';
            } else if (*p_set2 == 'b') {
                set2_translated[set2_count] = '\b';
            } else if (*p_set2 == 'f') {
                set2_translated[set2_count] = '\f';
            } else if (*p_set2 == 'n') {
                set2_translated[set2_count] = '\n';
            } else if (*p_set2 == 'r') {
                set2_translated[set2_count] = '\r';
            } else if (*p_set2 == 't') {
                set2_translated[set2_count] = '\t';
            } else if (*p_set2 == 'v') {
                set2_translated[set2_count] = '\v';
            } else if (*p_set2 == 39) {
                set2_translated[set2_count] = '\'';
            } else if (*p_set2 == 34) {
                set2_translated[set2_count] = '\"';
            } else {
                return invalid_format;
            }
        } else if (*p_set2 == '-') {
            /* 첫 번째 문자 혹은 마지막 문자가 - 인 경우 혹은 이전 문자가 range의 마지막인 경우 */
            if (p_set2 == p_set2_start || *(p_set2 + 1) == '\0' ||
                (last_range_end_index != NULL && p_set2 - 1 == last_range_end_index)) {
                set2_translated[set2_count] = '-';
            } else {
                if (*(p_set2 + 1) == 92) {
                    p_set2++;
                    if (*(p_set2 + 1) == 92) {
                        next_char = '\\';
                    } else if (*(p_set2 + 1) == 'a') {
                        next_char = '\a';
                    } else if (*(p_set2 + 1) == 'b') {
                        next_char = '\b';
                    } else if (*(p_set2 + 1) == 'f') {
                        next_char = '\f';
                    } else if (*(p_set2 + 1) == 'n') {
                        next_char = '\n';
                    } else if (*(p_set2 + 1) == 'r') {
                        next_char = '\r';
                    } else if (*(p_set2 + 1) == 't') {
                        next_char = '\t';
                    } else if (*(p_set2 + 1) == 'v') {
                        next_char = '\v';
                    } else if (*(p_set2 + 1) == 39) {
                        next_char = '\'';
                    } else if (*(p_set2 + 1) == 34) {
                        next_char = '\"';
                    } else {
                        return invalid_format;
                    }
                } else {
                    next_char = *(p_set2 + 1);
                }
                prev_char = set2_translated[set2_count - 1];
                if (prev_char > next_char) {
                    return invalid_range;
                }
                for (i = 0; (char)(set2_translated[set2_count - 1]) < next_char; ++i) {
                    if (set2_count == BUFFER_SIZE - 1) {
                        return argument_too_long;
                    }
                    set2_translated[set2_count] = (char)(set2_translated[set2_count - 1] + 1);
                    set2_count++;
                }
                p_set2++;
                set2_count--;
                last_range_end_index = p_set2;
            }
        } else {
            set2_translated[set2_count] = *p_set2;
        }
        p_set2++;
        set2_count++;
    }
    if (set2_count < set1_count) {
        temp_char = set2_translated[set2_count - 1];
        while (set2_count != set1_count) {
            set2_translated[set2_count] = temp_char;
            set2_count++;
        }
        set2_translated[set2_count] = '\0';
    } else {
        set2_translated[set1_count] = '\0';
    }

    /* CHANGE CHAR FROM STDIN AND PRINT TO STDOUT */
    c = fgetc(stdin);
    while (c != EOF) {
        for (i = set1_count - 1; i >= 0; --i) {
            if (argc == 4) {
                if (tolower(set1_translated[i]) == tolower(c)) {
                    break;
                }
            } else {
                if (set1_translated[i] == c) {
                    break;
                }
            }
        }
        if (i < 0) {
            putchar(c);
        } else {
            putchar(set2_translated[i]);
        }
        c = fgetc(stdin);
    }
    return 0;
}

