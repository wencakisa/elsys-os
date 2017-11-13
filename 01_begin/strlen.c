#include <stdio.h>

size_t strlen(const char*);

int main() {
    printf("%ld\n", strlen("Hello World!")); // == 12
    printf("%ld\n", strlen("tHis iS a TEST stRing!..")); // == 24

    return 0;
}

size_t strlen(const char *str) {
    size_t i = 0;
    for (; str[i] != '\0'; i++);

    return i;
}
