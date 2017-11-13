#include <stdio.h>
#include <stdlib.h>

#define LEN 20

size_t strlen(const char*);
char* strdup(const char*);

size_t strlen(const char *str) {
    size_t i = 0;
    for (; str[i] != '\0'; i++);

    return i;
}

char* strdup(const char *str) {
    size_t str_len = strlen(str);
    char *result = (char*) malloc(sizeof(char) * str_len);

    for (size_t i = 0; i < str_len; i++) {
        result[i] = str[i];
    }

    return result;
}
