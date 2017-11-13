#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <limits.h>

#define FILENAME_INDEX 1
#define OFFSET_BYTES_INDEX 2
#define BYTES_TO_PRINT_INDEX 3

int main(int argc, char const *argv[]) {
    const char *filename = argv[FILENAME_INDEX];
    off_t offset_bytes   = atoi(argv[OFFSET_BYTES_INDEX]);
    off_t bytes_to_print = atoi(argv[BYTES_TO_PRINT_INDEX]);

    int fd = open(filename, O_RDONLY);

    if (fd < 0) {
        perror(filename);
        return -1;
    }

    off_t fd_size = lseek(fd, 0, SEEK_END);
    lseek(fd, 0, SEEK_SET);
    lseek(fd, offset_bytes, SEEK_CUR);

    char buffer[PIPE_BUF];
    ssize_t rresult;

    while ((rresult = read(fd, buffer, PIPE_BUF)) != 0) {
        if (rresult < 0) {
            perror("read");
            return -1;
        }

        ssize_t i = 0;
        for (; i < rresult; i++) {
            if (i == bytes_to_print || i == fd_size) {
                break;
            }

            write(STDOUT_FILENO, buffer + i, 1);
        }

        if (i == bytes_to_print || i == fd_size) {
            break;
        }
    }

    if (close(fd) < 0) {
        perror("close");
        return -1;
    }

    return 0;
}
