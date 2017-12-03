#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define REPETITONS_PARAM_INDEX 1
#define PATH_PARAM_INDEX 2

int main(int argc, char const *argv[]) {
    size_t repetitions = atoi(argv[REPETITONS_PARAM_INDEX]);
    const char* path = argv[PATH_PARAM_INDEX];

    pid_t pids[repetitions];
    for (size_t i = 0; i < repetitions; i++) {
        // Store each fork in an array
        pids[i] = fork();

        // Child process, execute command
        if (pids[i] == 0) {
            execlp(path, path, NULL);
        } else if (pids[i] < 0){
            perror("fork");
        }
    }

    // Wait each child process
    while (repetitions > 0) {
        waitpid(pids[repetitions--], NULL, 0);
    }

    return 0;
}
