#include <stdio.h>
#include <stdlib.h>

#define DEF_CAP 20

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

void stack_push(stack*, int);
void stack_pop(stack*);

int main(int argc, char const *argv[]) {
    stack st;
    stack_init(&st);

    for (size_t i = 0; i < st.capacity; i++) {
        stack_push(&st, i);
        printf("Pushed: %d\n", stack_top(&st));
    }

    for (size_t i = 0; i < st.capacity; i++) {
        printf("Popped: %d\n", stack_top(&st));
        stack_pop(&st);
    }

    stack_destroy(&st);

    return 0;
}

void stack_init(stack* st) {
    st->size = 0;
    st->capacity = DEF_CAP;
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

void stack_push(stack* st, int n) {
    if (!stack_full(st)) {
        st->elements[st->size++] = n;
    }
}

void stack_pop(stack* st) {
    if (!stack_empty(st)) {
        st->elements[st->size--] = 0;
    }
}
