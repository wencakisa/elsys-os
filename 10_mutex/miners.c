//--------------------------------------------
// NAME: Vencislav Tashev
// CLASS: XIA
// NUMBER: 4
// PROBLEM: Miners
//---------------------------------------------

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>

#define DEFAULT_WORKERS_COUNT 1

#define MINERS_COUNT_INDEX 1
#define TRADERS_COUNT_INDEX 2

#define ACTIONS_PER_WORKER 20
#define GOLD_PER_ACTION 10

pthread_mutex_t mutex;

long gold = 0;

void* miner(void* arg) {
    int id = (intptr_t) arg;

    for (size_t i = 0; i < ACTIONS_PER_WORKER; i++) {
        pthread_mutex_lock(&mutex);

        gold += GOLD_PER_ACTION;
        printf("Miner %d gathered %d gold\n", id, GOLD_PER_ACTION);

        pthread_mutex_unlock(&mutex);

        sleep(2);
    }

    return NULL;
}

void* trader(void* arg) {
    int id = (intptr_t) arg;

    for (size_t i = 0; i < ACTIONS_PER_WORKER; i++) {
        pthread_mutex_lock(&mutex);

        if (gold < GOLD_PER_ACTION) {
            printf("The warehouse is empty, cannot sell!\n");
        } else {
            gold -= GOLD_PER_ACTION;
            printf("Trader %d sold %d gold\n", id, GOLD_PER_ACTION);
        }

        pthread_mutex_unlock(&mutex);

        sleep(2);
    }

    return NULL;
}

int main(int argc, char* argv[]) {
    int miners_count = DEFAULT_WORKERS_COUNT;
    int traders_count = DEFAULT_WORKERS_COUNT;

    if (argc >= 3) {
        miners_count = strtol(argv[MINERS_COUNT_INDEX], NULL, 10);
        traders_count = strtol(argv[TRADERS_COUNT_INDEX], NULL, 10);
    }

    pthread_t miners[miners_count], traders[traders_count];

    pthread_mutex_init(&mutex, NULL);

    for (size_t i = 0; i < miners_count; i++) {
        pthread_create(&miners[i], NULL, miner, (void*)(i + 1));
    }

    for (size_t i = 0; i < traders_count; i++) {
        pthread_create(&traders[i], NULL, trader, (void*)(i + 1));
    }

    for (size_t i = 0; i < miners_count; i++) {
        pthread_join(miners[i], NULL);
    }

    for (size_t i = 0; i < traders_count; i++) {
        pthread_join(traders[i], NULL);
    }

    printf("Gold: %ld\n", gold);

    pthread_mutex_destroy(&mutex);

    return 0;
}
