#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define BUFF_SIZE 100

int main(int argc, char const *argv[]) {
    const char *filename = argv[1];

    char buff[BUFF_SIZE];
    ssize_t rresult;
    ssize_t wresult;

    int fd = open(filename, O_RDONLY);

    if (fd < 0) {
        perror("open");
        return -1;
    }

    while((rresult = read(fd, buff, BUFF_SIZE)) != 0) {
        wresult = 0;

        if (rresult < 0) {
            perror("read");
            return -1;
        }

        while (wresult != rresult) {
            ssize_t res = write(STDOUT_FILENO, buff + wresult, rresult - wresult);

            if (wresult < 0) {
                perror("write");
                return -1;
            }

            wresult += res;
        }
    }

    if (close(fd) < 0) {
        perror("close");
        return -1;
    }

    return 0;
}
