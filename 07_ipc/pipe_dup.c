#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <stdio.h>

int main() {
    int pipefd[2];
    pipe(pipefd);

    pid_t pid = fork();

    if (pid == 0) {
        close(pipefd[1]);

        char ch;
        while (read(pipefd[0], &ch, 1) > 0) {
            write(STDOUT_FILENO, &ch, 1);
        }
        close(pipefd[0]);
    } else {
        close(pipefd[0]);
        dup2(pipefd[1], STDOUT_FILENO);
        printf("Hello world!\n");
        close(pipefd[1]);
    }

    return 0;
}
