#include <pthread.h>
#include <unistd.h>
#include <stdio.h>

#define BUYERS_COUNT 20
#define CARS_COUNT   5

pthread_mutex_t cars[CARS_COUNT];

void* test_car(void* arg) {
    for (size_t buyerId = 0; buyerId < BUYERS_COUNT; buyerId++) {
        for (size_t carId = 0; carId < CARS_COUNT; carId++) {
            if (!pthread_mutex_trylock(&cars[carId])) {
                printf("Buyer %ld takes car %ld.\n", buyerId + 1, carId + 1);
                printf("Buyer %ld returns car %ld.\n", buyerId + 1, carId + 1);
            }
        }
    }

    return NULL;
}

int main(int argc, char* argv[]) {
    for (size_t i = 0; i < CARS_COUNT; i++) {
        pthread_mutex_init(&cars[i], NULL);
    }

    pthread_t buyers[BUYERS_COUNT];

    for (size_t i = 0; i < BUYERS_COUNT; i++) {
        pthread_create(&buyers[i], NULL, test_car, NULL);
    }

    for (size_t i = 0; i < CARS_COUNT; i++) {
        pthread_mutex_destroy(&cars[i]);
    }

    return 0;
}
