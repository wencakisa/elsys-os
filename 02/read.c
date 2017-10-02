#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>


void *read_image(char *path) {
        int fd = open(path, O_RDONLY);
        if (fd < 0) {
            return NULL;
        }
        size_t size = 1000;
        size_t offset = 0;
        size_t res;
        char *buff = malloc(size);

        while((res = read(fd, buff + offset, 100)) != 0) {
                offset += res;
                if (offset + 100 > size) {
                        size *= 2;
                        buff = realloc(buff, size);
                }
        }
        close(fd);
        return buff;
}
