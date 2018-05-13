#include <unistd.h>
#include <stdbool.h>
#include <pthread.h>

#include "ui.h"

#define EXIT_COMMAND              'q'
#define INIT_MINER_COMMAND        'm'
#define INIT_SOLDIER_COMMAND      's'
#define INIT_10X_SOLDIERS_COMMAND 'x'

#define INIT_GOLD             100
#define INIT_HEALTH           100
#define INIT_ZOMBIES          1
#define INIT_ZOMBIES_DISTANCE 5
#define INIT_SOLDIERS         0
#define GOLD_PER_ACTION       10
#define PRICE_PER_SOLDIER     10
#define PRICE_PER_MINER       100

pthread_mutex_t mutex;

long gold = INIT_GOLD;
long health = INIT_HEALTH;

long zombies_count = INIT_ZOMBIES;
long soldiers_count = INIT_SOLDIERS;

void* miner(void* arg) {
    pthread_mutex_lock(&mutex);

    if (gold < PRICE_PER_MINER) {
        print_fail("Not enough gold!");
        pthread_mutex_unlock(&mutex);
        return NULL;
    }

    print_msg("Miner created!");

    gold -= PRICE_PER_MINER;
    print_gold(gold);

    pthread_mutex_unlock(&mutex);

    while (true) {
        pthread_mutex_lock(&mutex);

        gold += GOLD_PER_ACTION;
        print_gold(gold);

        pthread_mutex_unlock(&mutex);

        sleep(1);
    }

    return NULL;
}

void* zombie(void* arg) {
    while (true) {
        for (size_t i = INIT_ZOMBIES_DISTANCE; i > 0; i--) {
            print_zombies(i, zombies_count);
            sleep(1);
        }

        pthread_mutex_lock(&mutex);

        if (soldiers_count < zombies_count) {
            print_fail("Zombie attack succeeded ;(!");

            health -= zombies_count - soldiers_count;
            print_health(health);

            if (health <= 0) {
                game_end(zombies_count);
                return NULL;
            }
        } else {
            print_succ("Zombie attack deflected! :)");
        }

        zombies_count *= 2;

        pthread_mutex_unlock(&mutex);
    }

    return NULL;
}

void create_soldiers(int count) {
    int total_price = PRICE_PER_SOLDIER * count;

    pthread_mutex_lock(&mutex);

    if (gold < total_price) {
        print_fail("Not enough gold!");
        pthread_mutex_unlock(&mutex);
        return;
    }

    if (count == 1) {
        print_msg("Soldier created!");
    } else {
        print_msg("10 x soldiers created!");
    }

    gold -= total_price;
    print_gold(gold);

    soldiers_count += count;
    print_soldiers(soldiers_count);

    pthread_mutex_unlock(&mutex);

    return;
}

int main() {
	init();

    pthread_mutex_init(&mutex, NULL);

    print_gold(gold);
    print_soldiers(soldiers_count);
    print_health(health);

    pthread_t zombies_thread;
    pthread_t miners_thread;

    pthread_create(&zombies_thread, NULL, zombie, NULL);

    while (true) {
		int ch = get_input();

        switch(ch) {
            case INIT_MINER_COMMAND:
                pthread_create(&miners_thread, NULL, miner, NULL);
                break;
            case INIT_SOLDIER_COMMAND:
                create_soldiers(1);
                break;
            case INIT_10X_SOLDIERS_COMMAND:
                create_soldiers(10);
                break;
			case EXIT_COMMAND:
				game_end(zombies_count);
				break;
		}
    }

    pthread_cancel(zombies_thread);
    pthread_cancel(miners_thread);

    pthread_mutex_destroy(&mutex);

    return 0;
}
