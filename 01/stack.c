#include <stdio.h>
#include <stdlib.h>

#define MAX_CAPACITY 100

typedef struct {
    size_t size;
    size_t capacity;
    int *elements;
} stack;

void stack_init(stack*);
void stack_destroy(stack*);

int stack_empty(stack*);
int stack_full(stack*);

int stack_top(stack*);

void stack_resize(stack*);

void stack_push(stack*, int);
void stack_pop(stack*);

int main() {
    stack st;
    stack_init(&st);

    for (size_t i = 0; i < MAX_CAPACITY + 5; i++) {
        stack_push(&st, i);
    }

    printf("Top: %d\n", stack_top(&st));

    stack_destroy(&st);

    return 0;
}

void stack_init(stack* st) {
    st->size = 0;
    st->capacity = MAX_CAPACITY;
    st->elements = (int*) calloc(st->capacity, sizeof(int));
}

void stack_destroy(stack* st) {
    free(st->elements);
    st->elements = NULL;
    st->size = 0;
    st->capacity = 0;
}

int stack_empty(stack* st) {
    return st->size == 0;
}

int stack_full(stack* st) {
    return st->size == st->capacity;
}

int stack_top(stack* st) {
    return stack_empty(st) ? -1 : st->elements[st->size - 1];
}

void stack_resize(stack* st) {
    st->capacity *= 2;
    st->elements = (int*) realloc(st->elements, st->capacity * sizeof(int));
}

void stack_push(stack* st, int n) {
    if (stack_full(st)) {
        stack_resize(st);
    }

    st->elements[st->size++] = n;
}

void stack_pop(stack* st) {
    if (!stack_empty(st)) {
        st->size--;
    }
}
