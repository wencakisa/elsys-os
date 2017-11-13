#include <stdio.h>
#include <stdlib.h>

#define SIZE 50

size_t strlen(const char*);
char* strcat(char*, const char*);

int main() {
    char str[SIZE] = "Hello World!";
    char dest[SIZE] = "Daimo!";

    printf("Source: %s %ld\n", str, strlen(str));
    printf("Destination: %s %ld\n", strcat(dest, str), strlen(dest));

    return 0;
}

size_t strlen(const char *str) {
    size_t i = 0;
    for (; str[i] != '\0'; i++);

    return i;
}

char* strcat(char *destination, const char *source) {
    size_t i = strlen(destination), j = 0;

    for (; j < strlen(source); j++) {
        destination[i + j] = source[j];
    }
    destination[i + j] = '\0';

    return destination;
}
