#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char const *argv[]) {
    pid_t pid = fork();

    if (pid == 0) {
        execlp("ls", "ls", NULL);
    } else if (pid > 0) {
        wait(NULL);
        printf("Parent process\n");
    } else {
        perror("fork");
    }

    return 0;
}
