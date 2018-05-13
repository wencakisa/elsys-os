#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <errno.h>
#include <pthread.h>

#define DEFAULT_MINERAL_BLOCKS_COUNT     2
#define DEFAULT_SCVS_COUNT               5
#define MINERALS_PER_SCV                 8
#define MAX_MARINES_COUNT               20
#define MINERALS_PER_UNIT_PRICE         50
#define DEFAULT_MINERAL_BLOCK_MINERALS 500

#define INIT_MARINE_COMMAND 'm'
#define INIT_SCV_COMMAND    's'

typedef struct command_center_t {
    pthread_mutex_t mutex;
    int minerals;
    int empty_mineral_blocks;
    int scvs_count;
    int marines_count;
} command_center_t;

typedef struct mineral_block_t {
    pthread_mutex_t mutex;
    int minerals;
    int id;
} mineral_block_t;

typedef struct scv_t {
    int id;
} scv_t;

typedef struct unit_argument_t {
    int mineral_blocks_count;
    mineral_block_t* mineral_blocks;
} unit_argument_t;

typedef struct scv_argument_t {
    scv_t scv;
    unit_argument_t unit_argument;
} scv_argument_t;

command_center_t command_center;
pthread_t* scvs;

void init_command_center(command_center_t* command_center) {
    int status = pthread_mutex_init(&command_center->mutex, NULL);

    if (status != 0) {
        printf("pthread_mutex_init error\n");
        exit(status);
    }

    command_center->minerals = 0;
    command_center->empty_mineral_blocks = 0;
    command_center->scvs_count = DEFAULT_SCVS_COUNT;
    command_center->marines_count = 0;
}

void init_mineral_block(mineral_block_t* mineral_block, int id) {
    int status = pthread_mutex_init(&mineral_block->mutex, NULL);

    if (status != 0) {
        printf("pthread_mutex_init error\n");
        exit(status);
    }

    mineral_block->minerals = DEFAULT_MINERAL_BLOCK_MINERALS;
    mineral_block->id = id;
}

void destroy_command_center(command_center_t* command_center) {
    pthread_mutex_destroy(&command_center->mutex);
}

void destroy_mineral_block(mineral_block_t* mineral_block) {
    pthread_mutex_destroy(&mineral_block->mutex);
}

void* scv_routine(void* arg) {
    scv_argument_t* scv_argument = (scv_argument_t*) arg;

    scv_t scv = scv_argument->scv;

    unit_argument_t unit_argument = scv_argument->unit_argument;

    int mineral_blocks_count = unit_argument.mineral_blocks_count;
    mineral_block_t* mineral_blocks = unit_argument.mineral_blocks;

    size_t i = 0;

    while (true) {
        pthread_mutex_lock(&command_center.mutex);

        if (command_center.empty_mineral_blocks >= mineral_blocks_count) {
            pthread_mutex_unlock(&command_center.mutex);
            break;
        }

        pthread_mutex_unlock(&command_center.mutex);

        sleep(3);

        if (pthread_mutex_trylock(&mineral_blocks[i].mutex) == EBUSY) {
            ++i;
            i %= mineral_blocks_count;

            continue;
        } else {
            int current_minerals = mineral_blocks[i].minerals;
            int minerals_to_mine = 0;

            if (current_minerals > 0) {
                minerals_to_mine = (current_minerals < MINERALS_PER_SCV) ? current_minerals : MINERALS_PER_SCV;


                mineral_blocks[i].minerals -= minerals_to_mine;
                pthread_mutex_unlock(&mineral_blocks[i].mutex);

                if (minerals_to_mine > 0) {
                    printf("SCV %d is mining from mineral block %d\n", scv.id, mineral_blocks[i].id);
                    printf("SCV %d is transporting minerals\n", scv.id);

                    sleep(2);

                    pthread_mutex_lock(&command_center.mutex);
                    command_center.minerals += minerals_to_mine;

                    if (minerals_to_mine == current_minerals) {
                        command_center.empty_mineral_blocks++;
                    }

                    pthread_mutex_unlock(&command_center.mutex);

                    printf("SCV %d delivered minerals to the Command Center\n", scv.id);
                } else {
                    pthread_mutex_lock(&command_center.mutex);
                    command_center.empty_mineral_blocks++;
                    pthread_mutex_unlock(&command_center.mutex);
                }
            }
        }
    }

    return NULL;
}

void init_marine() {
    pthread_mutex_lock(&command_center.mutex);
    if (command_center.minerals >= MINERALS_PER_UNIT_PRICE) {
        command_center.marines_count++;
        command_center.minerals -= MINERALS_PER_UNIT_PRICE;
        pthread_mutex_unlock(&command_center.mutex);

        sleep(1);

        printf("You wanna piece of me, boy?\n");
    } else {
        printf("Not enough minerals.\n");
        pthread_mutex_unlock(&command_center.mutex);
    }
}

void init_scv(unit_argument_t* unit_argument) {
    pthread_mutex_lock(&command_center.mutex);

    if (command_center.minerals > MINERALS_PER_UNIT_PRICE) {
        pthread_mutex_unlock(&command_center.mutex);

        sleep(4);

        pthread_mutex_lock(&command_center.mutex);
        scvs = realloc(scvs, ++command_center.scvs_count * sizeof(pthread_t));

        scv_t scv = {
            .id = command_center.scvs_count
        };
        scv_argument_t scv_arguments = {
            .scv = scv,
            .unit_argument = *unit_argument
        };

        int status = pthread_create(
            &scvs[command_center.scvs_count - 1], NULL, scv_routine, (void*) &scv_arguments
        );

        if (status != 0) {
            printf("pthread_create error\n");
            exit(status);
        }

        printf("SCV good to go, sir.\n");
    } else {
        printf("Not enough minerals.\n");
    }

    pthread_mutex_unlock(&command_center.mutex);
}

void* player(void* arg) {
    unit_argument_t* unit_argument = (unit_argument_t*) arg;


    while (true) {
        pthread_mutex_lock(&command_center.mutex);

        if (command_center.marines_count >= MAX_MARINES_COUNT) {
            pthread_mutex_unlock(&command_center.mutex);
            break;
        }

        pthread_mutex_unlock(&command_center.mutex);

        char ch = getchar();

        switch (ch) {
            case INIT_MARINE_COMMAND:
                init_marine();
                break;
            case INIT_SCV_COMMAND:
                init_scv(unit_argument);
                break;
        }
    }

    return NULL;
}

int main(int argc, const char* argv[]) {
    int status = 0;

    init_command_center(&command_center);

    pthread_mutex_lock(&command_center.mutex);
    scvs = (pthread_t*) malloc(sizeof(pthread_t) * command_center.scvs_count);
    pthread_mutex_unlock(&command_center.mutex);

    int mineral_blocks_count = (argc > 1) ? atoi(argv[1]) : DEFAULT_MINERAL_BLOCKS_COUNT;

    mineral_block_t mineral_blocks[mineral_blocks_count];
    for (size_t i = 0; i < mineral_blocks_count; i++) {
        init_mineral_block(&mineral_blocks[i], i + 1);
    }

    for (size_t i = 0; i < command_center.scvs_count; i++) {
        scv_t scv = { .id = i + 1 };
        unit_argument_t unit_argument = {
            .mineral_blocks_count = mineral_blocks_count,
            .mineral_blocks = mineral_blocks
        };
        scv_argument_t scv_arguments = {
            .scv = scv,
            .unit_argument = unit_argument
        };

        status = pthread_create(&scvs[i], NULL, scv_routine, (void*) &scv_arguments);

        if (status != 0) {
            printf("pthread_create error\n");
            return status;
        }
    }

    pthread_t player_thread;
    unit_argument_t player_argument = {
        .mineral_blocks_count = mineral_blocks_count,
        .mineral_blocks = mineral_blocks
    };

    status = pthread_create(&player_thread, NULL, player, (void*) &player_argument);

    if (status != 0) {
        printf("pthread_create error\n");
        return status;
    }

    for (size_t i = 0; i < command_center.scvs_count; i++) {
        status = pthread_join(scvs[i], NULL);

        if (status != 0) {
            printf("pthread_join error\n");
            return status;
        }
    }

    status = pthread_join(player_thread, NULL);

    if (status != 0) {
        printf("pthread_join error\n");
        return status;
    }

    printf("Map minerals %d, ", mineral_blocks_count * DEFAULT_MINERAL_BLOCK_MINERALS);
    printf("player minerals %d, ", command_center.minerals);
    printf("SCVs %d, ", command_center.scvs_count);
    printf("Marines %d\n", command_center.marines_count);

    free(scvs);

    for (size_t i = 0; i < mineral_blocks_count; i++) {
        destroy_mineral_block(&mineral_blocks[i]);
    }

    destroy_command_center(&command_center);

    return status;
}
