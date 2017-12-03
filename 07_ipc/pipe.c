#include <unistd.h>
#include <fcntl.h>
#include <string.h>

int main() {
    const char* msg = "Hello World!\n";

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
        write(pipefd[1], msg, strlen(msg));
        close(pipefd[1]);
    }

    return 0;
}
