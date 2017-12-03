#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>
#include <stdbool.h>

#define DELIMITER " "

char** parse_cmdline(const char*);

int main() {
    while (true) {
        char cmdline[PIPE_BUF];

        read(STDIN_FILENO, cmdline, PIPE_BUF);
        // write(STDOUT_FILENO, cmdline, strlen(cmdline));

        char **parsed_line = parse_cmdline(cmdline);
        const char *filename = parsed_line[0];
        printf("Filename: %s\n", filename);
        // write(STDOUT_FILENO, filename, strlen(filename));

        // for (size_t i = 1; i < sizeof(parsed_line) / sizeof(parsed_line[0]); i++) {
        //     printf("bom\n");
        //     write(STDOUT_FILENO, parsed_line[i], strlen(parsed_line[i]));
        // }

        free(parsed_line);
    }

    return 0;
}

char** parse_cmdline(const char *cmdline) {
    printf("%s\n", cmdline);

    char **result = (char**) calloc(strlen(cmdline), sizeof(char*));

    char *cmd_copy = (char*) calloc(strlen(cmdline) + 1, sizeof(char));
    strcpy(cmd_copy, cmdline);

    size_t i = 0;
    char *token = strtok(cmd_copy, DELIMITER);
    while (token != NULL) {
        strcpy(result[i++], token);
        token = strtok(NULL, DELIMITER);
    }

    free(cmd_copy);

    return result;
}
