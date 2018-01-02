//------------------------------------------------------------------------
// NAME: Vencislav Tashev
// CLASS: XIa
// NUMBER: 4
// PROBLEM: #2
// FILE NAME: shell.c
// FILE PURPOSE:
// A Unix-like shell command interpreter
// written in C using processes and exec* functions
//------------------------------------------------------------------------

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

void free_string_array(char** arr);
size_t count(const char* haystack, const char needle);
char** parse_cmdline(const char* cmdline);
void run_command(char** command);

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

        // Running the already parsed command line
        run_command(parse_cmdline(buffer));
    }
}

//------------------------------------------------------------------------
// FUNCTION: free_string_array
// Completely frees space for a dynamically allocated char** (array of strings)
// PARAMETERS:
// char** arr - The array to be freed
//------------------------------------------------------------------------
void free_string_array(char** arr) {
    for (int i = 0; arr[i] != '\0'; i++) {
        // free each previously allocated element in the array
        free(arr[i]);
    }
    // free the whole array pointer
    free(arr);
    // set the array pointer to point to NULL
    arr = NULL;
}

//------------------------------------------------------------------------
// FUNCTION: count
// Counts a char encounters in a string
// PARAMETERS:
// const char* haystack - The string that we search in
// const char needle    - The char that we search for
//------------------------------------------------------------------------
size_t count(const char* haystack, const char needle) {
    size_t result = 0;
    for (size_t i = 0; i < strlen(haystack); i++) {
        if (haystack[i] == needle) {
            result++;
        }
    }

    return result;
}

//------------------------------------------------------------------------
// FUNCTION: parse_cmdline
// Parses a provided line from the input
// by tokenizing it by space and
// returning the tokens in an dynamically allocated array
// PARAMETERS:
// const char* cmdline - The line to be parsed
//------------------------------------------------------------------------
char** parse_cmdline(const char* cmdline) {
    const size_t len = strlen(cmdline);

    // copy the provided constant cmdline
    // in order to tokenize it with strtok()
    char* line_copy = (char*) calloc(sizeof(char), len + 1);
    strcpy(line_copy, cmdline);

    // counting the encounters of ' ' in the string in order to determine
    // the size for the result array of strings
    const size_t spaces_count = count(line_copy, ' ');
    // the result array of strings
    // we allocate spaces_count + 2 memory for the null byte
    char** parsed = (char**) calloc(sizeof(char*), spaces_count + 2);

    // strtok() breaks provided string into copies by provided delimiter
    char* token = strtok(line_copy, DELIMITER);
    int i = 0;
    for (; token != NULL; i++) {
        // allocate space for the current element in the result array
        // and copying the current token value into it
        parsed[i] = (char*) calloc(sizeof(char), strlen(token) + 1);
        strcpy(parsed[i], token);

        // go to the next token
        token = strtok(NULL, DELIMITER);
    }
    // set a null byte in the end
    parsed[i] = '\0';

    // free the line copy because we do not need it anymore
    free(line_copy);

    return parsed;
}

//------------------------------------------------------------------------
// FUNCTION: run_command
// The main functionality of the program
// Runs a given command array of strings ( e.g. {"ls", "-la"} ) by:
// -> creating a process ( fork() ),
// -> executing the already parsed command in child process ( exec*() )
// PARAMETERS:
// char** command - The command line to be executed
//------------------------------------------------------------------------
void run_command(char** command) {
    // create a new process using fork()
    pid_t pid = fork();

    if (pid == 0) {
        // Child process

        execvp(command[0], command);
        // exec* will interrupt the child process
        // so the error handling can be placed immediately after exec*
        // when exec* fails, it will continue down the child process
        // and the error handling will execute correctly

        perror(command[0]);
        free_string_array(command);
        _exit(EXIT_FAILURE);
    } else if (pid > 0) {
        // Parent process

        // Wait for the child to finish it's work
        waitpid(pid, NULL, 0);
    } else {
        perror("fork");
    }

    // Free the already executed command, we do not need it anymore
    free_string_array(command);
}
