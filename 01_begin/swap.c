#include <stdio.h>

void swap(int*, int*);

int main() {
    int a = 5;
    int b = 3;

    printf("a = %d\n", a);
    printf("b = %d\n", b);

    swap(&a, &b);

    printf("a = %d\n", a);
    printf("b = %d\n", b);

    return 0;
}

void swap(int *a, int *b) {
    int c = *a;
    *a = *b;
    *b = c;
}
