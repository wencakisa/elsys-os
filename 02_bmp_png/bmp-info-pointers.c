#include <stdio.h>
#include <stdint.h>
#include "read.h"

#define FILE_ARGV_INDEX 1
#define FILE_WIDTH_BYTE 18
#define FILE_HEIGHT_BYTE 22

int main(int argc, char const *argv[]) {
    if (argc < 2) {
        printf("Usage: %s [filename].bmp\n", argv[0]);
        return 1;
    }

    const char *filename = argv[FILE_ARGV_INDEX];

    void *data = read_image(filename);
    uint8_t *begin = (uint8_t*) data;

    int32_t *width_addr = (int32_t *)(begin + FILE_WIDTH_BYTE);
    int32_t *height_addr = (int32_t *)(begin + FILE_HEIGHT_BYTE);

    int32_t width = *width_addr;
    int32_t height = *height_addr;

    printf("BMP image width: %d\n", width);
    printf("BMP image height: %d\n", height);

    return 0;
}
