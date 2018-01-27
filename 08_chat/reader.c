#include "chat.h"

int main() {
    initialize();

    int fd;

    while (1) {
        int receive_status = receive(fd);

        if (receive_status < 0) {
            return receive_status;
        }

        int send_status = send(fd);

        if (send_status < 0) {
            return send_status;
        }
    }

    return 0;
}
