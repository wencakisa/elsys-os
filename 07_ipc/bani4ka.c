#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>

#define READ_END 0
#define WRITE_END 1
#define MAX_PRODUCED 100

int main() {
    int pipefd[2];
    if (pipe(pipefd) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    pid_t pid = fork();

    if (pid == 0) {
        close(pipefd[WRITE_END]);

        char ch;
        while (read(pipefd[READ_END], &ch, 1) > 0) {
            write(STDOUT_FILENO, &ch, 1);
        }

        close(pipefd[READ_END]);
        _exit(EXIT_SUCCESS);
    } else if (pid > 0) {
        close(pipefd[READ_END]);

        dup2(pipefd[WRITE_END], STDOUT_FILENO);

        int id = 0;
        while (id <= MAX_PRODUCED) {
            printf("Bani4ka #%d\n", id);
            id++;
        }

        close(pipefd[WRITE_END]);

        wait(NULL);
        exit(EXIT_SUCCESS);
    } else {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    return 0;
}
