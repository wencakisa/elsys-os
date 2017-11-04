#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <limits.h>

#define LINES_TO_READ 10

int main() {
    const char *filename = "a.txt";

    int fd = open(filename, O_RDONLY);

    char buffer[PIPE_BUF];
    size_t lines = 0;
    ssize_t rresult;

    while ((rresult = read(fd, buffer, PIPE_BUF)) != 0) {
        for (size_t i = 0; i < rresult && lines < LINES_TO_READ; i++) {
            if (*(buffer + i) == '\n') {
                lines++;
            }

            write(STDOUT_FILENO, buffer + i, 1);
        }

        if (lines == LINES_TO_READ) {
            break;
        }
    }

    close(fd);

    return 0;
}
