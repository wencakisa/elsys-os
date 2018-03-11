#include <stdio.h>
#include <stdbool.h>
#include <pthread.h>

#define NUM_THREADS 10

long count = 0;
pthread_mutex_t mutex;

void *prime(void *arg) {
    int start = 10000 * ((int) arg);
    if (start < 2) {
        start = 2;
    }
    int end = 0;
    printf("Prime calculation started for N=%d\n", end);

    int local_count = 0;
    for (size_t i = start; i < end; i++) {
        bool prime = true;

        for (size_t j = 2; j < (i / 2) + 1; j++) {
            if (i % j == 0) {
                prime = false;
                break;
            }
        }

        if (prime) {
            local_count++;
        }
    }

    pthread_mutex_lock(&mutex);
    count += local_count;
    pthread_mutex_unlock(&mutex);
}

int main() {
    pthread_mutex_init(&mutex, NULL);

    pthread_t threads[NUM_THREADS];

    for (size_t i = 0; i < NUM_THREADS; i++) {
        pthread_create(&threads[i], NULL, prime, (void*) i);
    }

    for (size_t i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    pthread_mutex_destroy(&mutex);

    return 0;
}
