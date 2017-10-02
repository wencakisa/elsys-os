#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "read.h"

#define FILE_ARGV_INDEX 1
#define FILE_WIDTH_BYTE 18
#define FILE_HEIGHT_BYTE 22

int main(int argc, char const *argv[]) {
    // Check if the user provided a filename
    if (argc < 2) {
        printf("Usage: %s [filename].bmp\n", argv[0]);
        return 1;
    }

    const char *filename = argv[FILE_ARGV_INDEX];
    char *file = (char*) read_image(filename);

    int32_t width;
    // Create a 4-byte variable that will store the width
    memcpy(&width, file + FILE_WIDTH_BYTE, sizeof(width));
    // file + FILE_WIDTH_BYTE -> pointer to the (FILE_WIDTH_BYTE)th byte of the file
    // sizeof(width) = sizeof(int32_t) = 4
    // Copy into width 4 bytes after the (FILE_WIDTH_BYTE)th byte of the file!

    int32_t height;
    // Create a 4-byte variable that will store the height
    memcpy(&height, file + FILE_HEIGHT_BYTE, sizeof(height));
    // file + FILE_HEIGHT_BYTE -> pointer to the (FILE_HEIGHT_BYTE)th byte of the file
    // sizeof(width) = sizeof(int32_t) = 4
    // Copy into width 4 bytes after the (FILE_HEIGHT_BYTE)th byte of the file!

    printf("BMP image width: %d\n", width);
    printf("BMP image height: %d\n", height);

    return 0;
}
