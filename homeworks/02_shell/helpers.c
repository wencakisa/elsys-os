#include <stdlib.h>
#include <string.h>

#include "helpers.h"

void free_string_array(char** arr) {
    for (int i = 0; arr[i] != NULL; i++) {
        // free each previously allocated element in the array
        free(arr[i]);
    }
    // free the whole array pointer
    free(arr);
    // set the array pointer to point to NULL
    arr = NULL;
}

size_t count_char(const char* haystack, const char needle) {
    size_t result = 0;
    for (size_t i = 0; i < strlen(haystack); i++) {
        if (haystack[i] == needle) {
            result++;
        }
    }

    return result;
}

char** split_into_array(char* str, const char* delimiter) {
    // counting the encounters of delimiter[0] in the string
    // in order to determine the size for the result array of strings
    const size_t size = count_char(str, delimiter[0]);
    // allocate space for our result array
    // here we use the provided size + 2
    // because we always allocate space for the null byte + 1
    // e.g.
    //     str: "a|b|c"
    //     delimiter: "|"
    //     size: 2 (the encounters of delimiter)
    //     --> result: { "a", "b", "c", NULL }
    //     _____________________________^ Result size: size + 2
    char** result = (char**) calloc(sizeof(char*), size + 2);

    // strtok() breaks provided string into copies by provided delimiter
    char* token = strtok(str, delimiter);
    int i = 0;
    for (; token != NULL; i++) {
        // allocate space for the current element in the result array
        // and copying the current token value into it
        result[i] = (char*) calloc(sizeof(char), strlen(token) + 1);
        strcpy(result[i], token);

        // go to the next token
        token = strtok(NULL, delimiter);
    }
    // set a null byte in the end
    result[i] = NULL;

    return result;
}
