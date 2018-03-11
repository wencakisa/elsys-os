#include <stdio.h>
#include <pthread.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

#define MAX_LINE_LENGTH 256

#define START_COMMAND "p "
#define EXIT_COMMAND  "e"

#define NEW_LINE "\n"
#define SPACE " "

bool is_start_command(const char*);
bool is_exit_command(const char*);
void print_help();
void* count_primes(void*);

int main() {
    while (true) {
        char line[MAX_LINE_LENGTH];

        fgets(line, MAX_LINE_LENGTH, stdin);
        strtok(line, NEW_LINE);

        if (is_start_command(line)) {
            strtok(line, SPACE);
            const char* number_str = strtok(NULL, line);
            long number = (long) strtol(number_str, NULL, 10);

            pthread_t thread;
            pthread_create(&thread, NULL, count_primes, (void*) number);
        } else if (is_exit_command(line)) {
            break;
        } else {
            print_help();
        }
    }

    pthread_exit(NULL);

    return 0;
}

bool is_start_command(const char* token) {
    return !strncmp(token, START_COMMAND, strlen(START_COMMAND));
}

bool is_exit_command(const char* token) {
    return !strncmp(token, EXIT_COMMAND, strlen(EXIT_COMMAND));
}

void print_help() {
    printf("Supported commands:\n");
    printf("p N - Starts a new calculation for the number of primes from 1 to N\n");
    printf("e - Waits for all calculations to finish and exits\n");
}

void* count_primes(void *arg) {
    long number = (long) arg;

    printf("Prime calculation started for N=%ld\n", number);

    int count = 0;
    for (size_t i = 2; i <= number; i++) {
        bool is_prime = true;
        
        for (size_t j = 2; j < i; j++) {
            if (i % j == 0) {
                is_prime = false;
                break;
            }
        }

        if (is_prime) {
            count++;
        }
    }

    printf("Number of primes for N=%ld is %d\n", number, count);

    return NULL;
}
