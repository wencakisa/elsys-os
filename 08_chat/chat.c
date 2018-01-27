#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>

#include "constants.h"
#include "chat.h"

void initialize() {
    mkfifo(FIFO_PATH, 0666);
}

int send(int fd) {
    char message_to_send[BUFF_SIZE];
    fd = open(FIFO_PATH, O_WRONLY);

    if (fd < 0) {
        return -1;
    }

    printf("<sender>: ");

    if (fgets(message_to_send, BUFF_SIZE, stdin) == NULL) {
        return -1;
    }

    if (write(fd, message_to_send, strlen(message_to_send) + 1) < 0) {
        return -1;
    }

    close(fd);

    return 0;
}

int receive(int fd) {
    char message_to_receive[BUFF_SIZE];
    fd = open(FIFO_PATH, O_RDONLY);

    if (fd < 0) {
        return -1;
    }

    if (read(fd, message_to_receive, BUFF_SIZE) < 0) {
        return -1;
    }

    printf("<receiver>: %s", message_to_receive);
    close(fd);

    return 0;
}
