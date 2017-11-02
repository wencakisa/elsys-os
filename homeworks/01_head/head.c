#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <limits.h>

#define LINES_TO_READ 10
#define ARGC_TO_PRINT_NAMES 2
#define MAX_TITLE_LEN 50

void print_filename(const char*);
void head_file(int);

int main(int argc, char const *argv[]) {
    for (ssize_t i = 1; i < argc; i++) {
        const char *filename = argv[i];

        if (argc > ARGC_TO_PRINT_NAMES) {
            if (i >= ARGC_TO_PRINT_NAMES) {
                write(STDOUT_FILENO, "\n", 1);
            }

            print_filename(filename);
        }

        int fd = open(filename, O_RDONLY);

        if (fd < 0) {
            perror(filename);
            return -1;
        }

        head_file(fd);

        if (close(fd) < 0) {
            perror("close");
            return -1;
        }
    }

    return 0;
}

void print_filename(const char *filename) {
    char title[MAX_TITLE_LEN];
    sprintf(title, "==> %s <==\n", filename);

    write(STDOUT_FILENO, title, strlen(title));
}

void head_file(int fd) {
    char buffer[PIPE_BUF];
    size_t lines = 0;
    ssize_t rresult, wresult;

    while ((rresult = read(fd, buffer, PIPE_BUF)) != 0) {
        if (rresult < 0) {
            perror("read");
            exit(-1);
        }

        for (size_t j = 0; j < rresult && lines < LINES_TO_READ; j++, wresult = 0) {
            if (*(buffer + j) == '\n') {
                lines++;
            }

            ssize_t res = write(STDOUT_FILENO, buffer + j, 1);
            if (wresult < 0) {
                perror("write");
                exit(-1);
            }

            wresult += res;
        }

        if (lines == LINES_TO_READ) {
            break;
        }
    }
}
