#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>

int main() {
    int fd = open("output.txt", O_CREAT | O_WRONLY, S_IRWXU | S_IRWXG | S_IRWXO);

    dup2(fd, STDOUT_FILENO);

    if (fd < 0) {
        perror("open");
        return -1;
    }

    printf("Hello World!\n");
    
    close(fd);
    return 0;
}
