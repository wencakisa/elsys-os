#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void swap(void*, void*, size_t);

int main() {
    int a = 3, b = 5;

    swap(&a, &b, sizeof(int));

    printf("a = %d\n", a);
    printf("b = %d\n", b);

    char c = 'c', d = 'd';

    swap(&c, &d, sizeof(char));

    printf("c = %c\n", c);
    printf("d = %c\n", d);

    return 0;
}

void swap(void *a, void *b, size_t size) {
    void *temp = malloc(size);

    memcpy(temp, a, size);
    memcpy(a, b, size);
    memcpy(b, temp, size);

    free(temp);
}
