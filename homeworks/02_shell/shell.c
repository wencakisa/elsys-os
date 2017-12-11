#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>

#define LINE_PREFIX "$ "
#define BUFF_SIZE 4096
#define DELIMITER " "

char** parse_cmdline(const char* cmdline);
void free_string_array(char** arr);

int main() {
    char buffer[BUFF_SIZE];

    while (1) {
        printf(LINE_PREFIX);

        // break out of the loop when we reach EOF
        if (fgets(buffer, BUFF_SIZE, stdin) == NULL) {
            exit(EXIT_SUCCESS);
        }

        // fgets() puts a "\n" in the end of input
        // strcspn() scans str1 for the first occurrence of any of the characters that are part of str2,
        // returning the number of characters of str1 read before this first occurrence.
        buffer[strcspn(buffer, "\n")] = '\0';

        char** parsed_line = parse_cmdline(buffer);
        const char* path = parsed_line[0];

        pid_t pid = fork();

        if (pid == 0) {
            execvp(path, parsed_line);
            // exec* will interrupt the child process
            // so the error handling can be placed immediately after exec*
            // when exec* fails, it will continue down the child process
            // and the error handling will execute correctly

            perror(path);
            free_string_array(parsed_line);
            _exit(EXIT_FAILURE);
        } else if (pid > 0) {
            wait(NULL);
            free_string_array(parsed_line);
        } else {
            perror("fork");
            free_string_array(parsed_line);
            exit(EXIT_FAILURE);
        }
    }
}

void free_string_array(char** arr) {
    for (int i = 0; arr[i] != '\0'; i++) {
        // clear each previously allocated element in the array
        free(arr[i]);
    }
    // clear the whole array pointer
    free(arr);
}

char** parse_cmdline(const char* cmdline) {
    const size_t len = strlen(cmdline);

    // copy the provided constant cmdline
    // in order to tokenize it with strtok()
    char* line_copy = (char*) calloc(sizeof(char), len + 1);
    strcpy(line_copy, cmdline);

    // the result array of strings (char**)
    char** parsed = (char**) calloc(sizeof(char*), len);

    // strtok() breaks provided string into copies by provided delimiter
    char* token = strtok(line_copy, DELIMITER);
    int i = 0;
    for (; token != NULL; i++) {
        // allocate space for the current element in the result array
        parsed[i] = (char*) calloc(sizeof(char), strlen(token) + 1);
        // copy the token value into it
        strcpy(parsed[i], token);

        token = strtok(NULL, DELIMITER);
    }
    // set a null byte in the end
    parsed[i] = '\0';

    free(line_copy);

    return parsed;
}
