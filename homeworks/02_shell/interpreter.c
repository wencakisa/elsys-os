#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>

#include "helpers.h"
#include "parser.h"
#include "interpreter.h"

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

        // wait for the child to finish its work
        waitpid(pid, NULL, 0);
    } else {
        // handle forking error
        perror("fork");
    }

    free_string_array(command);
}

void close_pipe(int pipefd[2]) {
    close(pipefd[READ_END]);
    close(pipefd[WRITE_END]);
}

void run_piped_commands(char** current, char** next) {
    int pipefd[2];

    if (pipe(pipefd) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    pid_t pid = fork();

    if (pid == 0) {
        // Child process
        dup2(pipefd[WRITE_END], STDOUT_FILENO);
        close_pipe(pipefd);

        execvp(current[0], current);

        perror(current[0]);
        free_string_array(current);
        _exit(EXIT_FAILURE);
    } else if (pid > 0) {
        // Create second child process
        pid = fork();

        if (pid == 0) {
            dup2(pipefd[READ_END], STDIN_FILENO);
            close_pipe(pipefd);

            execvp(next[0], next);

            perror(next[0]);
            free_string_array(next);
            _exit(EXIT_FAILURE);
        } else if (pid > 0) {
            close_pipe(pipefd);
            waitpid(pid, NULL, 0);
        } else {
            perror("fork");
        }
    } else {
        perror("fork");
    }

    close_pipe(pipefd);
    free_string_array(current);
}

void execute_shell(const char* prefix) {
    char buffer[BUFF_SIZE];

    while (1) {
        printf("%s", prefix);

        // break out of the loop when we reach EOF
        if (fgets(buffer, BUFF_SIZE, stdin) == NULL) {
            exit(EXIT_SUCCESS);
        }

        // fgets() puts a "\n" in the end of input
        // strcspn() scans str1 for the first occurrence of any of the characters that are part of str2,
        // returning the number of characters of str1 read before this first occurrence.
        buffer[strcspn(buffer, "\n")] = '\0';

        const size_t pipes_count = count_char(buffer, PIPE_SYMBOL[0]);

        // here we have no pipes, so we run command as normal one
        if (pipes_count == 0) {
            run_command(parse_cmdline(buffer));
        } else {
            char** parsed_pipes = parse_pipes(buffer);

            for (size_t i = 0; i < pipes_count; i++) {
                // Running each two commands from the already pipe parsed line
                run_piped_commands(
                    parse_cmdline(parsed_pipes[i]),
                    parse_cmdline(parsed_pipes[i + 1])
                );
            }

            free_string_array(parsed_pipes);
        }
    }
}
