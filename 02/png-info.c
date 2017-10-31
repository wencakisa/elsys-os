//------------------------------------------------------------------------
// NAME: Vencislav Tashev
// CLASS: XIa
// NUMBER: 4
// PROBLEM: #2
// FILE NAME: png-info.c
// FILE PURPOSE:
//      Take a file name from the command line (a PNG file)
//      Validate if the file is accessible
//      And print it's width, height and file size on the console
//------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include "read.h"

/*
Важни детайли от условието:
    => PNG форматът се състои от начална сигнатура, състояща се от последователността от байтове 89 50 4E 47 0D 0A 1A 0A (8 байта), и последователност от блокове (chunks).
    => Един chunk има следния формат: chunk length (4 байта), chunk type (4 байта), chunk data (chunk length на брой байта), chunk crc (4 байта)
Решение:
    => Трябва да skip-нем chunk_header (8 байта) + chunk_length (4 байта) + chunk_type (още 4 байта) = 8 + 4 + 4 = 16 байта
    => След този 16-ти байт следват две парчета по 4 байта, всяко от което съдържа width & height на .png файла
    => За width: Записваме в променлива от тип uint32_t (const 4 байта) първите 4 байта след 16-тия (16-20)
    => За height: Записваме в променлива от тип uint32_t (const 4 байта) следващите 4 байта след 20-тия (20-24)
Забележки:
    Всички числа във файла (chunk length, image width, image height) са в т. нар. network byte order, което означава,
    че числото 16 (десетично) се записва в паметта/изпраща по мрежата/записва във файл в следната последователност от байтове 00 00 00 10.
    Но вашите компютри може да работят с числа подредени по друг начин (чисслото 16 може да е представен като 10 00 00 00).

    За да се справите с този проблем, използвайте функцията ntohl (декларирана в заглавния файл <arpa/inet.h>),
    която конвертира от network byte order във формата, който използва вашата машина.
*/

#define FILE_ARGV_INDEX 1
#define EXTENSION "png"
#define PNG_HEADER_BYTES 8
#define CHUNK_LENGTH_BYTES 4
#define CHUNK_TYPE_BYTES 4
// В SKIP_BYTES пресмятаме колко байта трябва да се "прескочат",
// докато се стигне до съществената за нас информация, а именно width & height на .png файла
#define SKIP_BYTES PNG_HEADER_BYTES + CHUNK_LENGTH_BYTES + CHUNK_TYPE_BYTES

//------------------------------------------------------------------------
// FUNCTION: validate_png_filename
//      Validates if a given PNG file exists and is readable
// PARAMETERS:
//      const char *filename - The path for the PNG file
//------------------------------------------------------------------------
void validate_png_filename(const char*);

//------------------------------------------------------------------------
// FUNCTION: get_file_extension
//      Returns the extension of a given filename (if there is one)
// PARAMETERS:
//      const char *filename - The path to the file
//------------------------------------------------------------------------
const char *get_file_extension(const char*);

//------------------------------------------------------------------------
// FUNCTION: get_bytes_from_nbo
//      Returns bytes converted from network byte order to our system's one
//      From a void* with a given offset
// PARAMETERS:
//      void *src     - The pointer to our byte sequence
//      size_t offset - Bytes to be skipped
//------------------------------------------------------------------------
uint32_t get_bytes_from_nbo(void*, size_t);

//------------------------------------------------------------------------
// FUNCTION: get_file_size
//      Returns the size of a file (in bytes)
// PARAMETERS:
//      const char *filename - The path to the file
//------------------------------------------------------------------------
uint32_t get_file_size(const char*);

int main(int argc, char const *argv[]) {
    if (argc < 2) {
        printf("Usage: %s [filename].%s\n", argv[0], EXTENSION);
        return 1;
    }

    const char *filename = argv[FILE_ARGV_INDEX];
    validate_png_filename(filename);

    void *image = read_image(filename);

    uint32_t width = get_bytes_from_nbo(image, SKIP_BYTES);
    uint32_t height = get_bytes_from_nbo(image, SKIP_BYTES + sizeof(width));
    uint32_t image_size = get_file_size(filename);

    printf("PNG image width: %d\n", width);
    printf("PNG image height: %d\n", height);
    printf("PNG file size: %d\n", image_size);

    return 0;
}

void validate_png_filename(const char *filename) {
    // Проверка дали файлът съществува и дали имаме право да го четем
    if (access(filename, F_OK) == -1 || access(filename, R_OK) == -1) {
        printf("Unable to open file\n");
        exit(1);
    }
    // Проверка дали разширението на файла е същото като дефинирания EXTENSION
    if (strcmp(get_file_extension(filename), EXTENSION)) {
        printf("Unable to parse file\n");
        exit(1);
    }

    // Ако се изпълни който и да е от двата условни оператора,
    // програмата ще приключи изпълнението си със exit_code=1
}

const char *get_file_extension(const char *filename) {
    // За да сме максимално сигурни, че потребителя ще ни подаде снимка в правилния формат,
    // Имплементираме функция, която проверява разширението на дадено име на файл.

    // За целта може да използваме функцията strrchr(const char *str, int character)
    // From the C++ docs: Locate last occurrence of character in string
    const char *dot = strrchr(filename, '.');

    // Ако сме успели да split-нем името на файла по '.',
    // връщаме указател към следващия елемент (което е на практика разширението на файла).
    // В останалите случаи, връщаме празен низ.
    return (dot) ? dot + 1 : "";
}

uint32_t get_bytes_from_nbo(void *src, size_t offset) {
    uint32_t result;

    // Записваме в result толкова байта, колкото е size (в случая колкото е размера на uint32_t (който е 4))
    // Като skip-нем SKIP_BYTES (16) байта след началото на файла.
    memcpy(&result, (char*) src + offset, sizeof(result));
    // Тъй като в момента стойността на result e в т.нар. network byte order (виж забележките),
    // изплозваме функцията ntohl от <arpa/inet.h> за да го представим в такъв вид,
    // какъвто изпозлва нашия компютър.
    result = ntohl(result);

    return result;
}

uint32_t get_file_size(const char *filename) {
    // За определяне размера на файл (в байтове), може да се изпозлва функцията ftell(FILE *strea) от стандартната библиотека:
    // From the C++ docs: Get current position in stream
    FILE *f = fopen(filename, "r");

    // Първо, преместваме позицията, към която сочи нашия файл, да е в неговия край (чрез функцията fseek)
    fseek(f, 0, SEEK_END);
    // След това извикваме ftell() и на практика това е размера на нашия файл
    uint32_t size = ftell(f);
    // Накрая отново връщаме указателя да сочи към началото на файла (отново чрез fseek)
    fseek(f, 0, SEEK_SET);
    fclose(f);

    return size;
}
