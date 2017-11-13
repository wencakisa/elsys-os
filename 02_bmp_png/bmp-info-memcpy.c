#include <stdio.h>
#include <stdint.h>
#include <string.h>
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

    uint8_t *file = (uint8_t*) read_image(filename);

    uint32_t width;
    memcpy(&width, file + FILE_WIDTH_BYTE, sizeof(width));

    uint32_t height;
    memcpy(&height, file + FILE_HEIGHT_BYTE, sizeof(height));

    printf("BMP image width: %d\n", width);
    printf("BMP image height: %d\n", height);

    return 0;
}
