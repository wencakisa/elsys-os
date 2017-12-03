#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>

#define BUFF_SIZE 4096
#define DELIMITER " "

bool is_file(const char* path);
char** parse_cmdline(const char* cmdline);

int main() {
    char buffer[BUFF_SIZE];

    while (true) {
        printf("$");

        fgets(buffer, BUFF_SIZE, stdin);
        strtok(buffer, "\n");

        char** parsed_line = parse_cmdline(buffer);
        const char* path = parsed_line[0];

        // if (!is_file(path)) {
        //     printf("%s: No such file or directory\n", path);
        //     return -1;
        // }

        pid_t pid = fork();

        if (pid == 0) {
            if (execvp(path, parsed_line) == -1) {
                perror(path);
            }

            return -1;
        } else if (pid > 0) {
            waitpid(pid, NULL, 0);
        } else {
            perror("fork");
        }

        free(parsed_line);
    }

    return 0;
}

bool is_file(const char* path) {
    struct stat buf;
    stat(path, &buf);
    return S_ISREG(buf.st_mode);
}

char** parse_cmdline(const char* cmdline) {
    const size_t len = strlen(cmdline);

    char** parsed = (char**) calloc(sizeof(char*), len + 1);

    char* line = (char*) calloc(sizeof(char), len + 1);
    strcpy(line, cmdline);

    char* token = strtok(line, DELIMITER);
    int i = 0;
    while(token != NULL) {
        parsed[i] = (char*) calloc(strlen(token) + 1, sizeof(char));
        strcpy(parsed[i], token);

        token = strtok(NULL, DELIMITER);
        i++;
    }

    free(line);

    return parsed;
}
