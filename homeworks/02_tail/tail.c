//------------------------------------------------------------------------
// NAME: Vencislav Tashev
// CLASS: XIa
// NUMBER: 4
// PROBLEM: #1
// FILE NAME: tail.c
// FILE PURPOSE:
// Implements the UNIX tail command
// using only system functions
// like write(), read(), open(), lseek()
//------------------------------------------------------------------------

#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <limits.h>

// Error message representation functions are in this file
#include "errors.h"

#define LINES_TO_READ 10
#define ARGC_TO_PRINT_NAMES 2
#define STDIN_AS_FILENAME "-"

void print_filename_as_title(const char*);
void read_from_stdin();
void offset_to_last_n_lines(int, int);
void cat_file(int, const char*);
void tail_file(int, const char*);

int main(int argc, char const *argv[]) {
    if (argc > 1) {
        size_t valid_filenames = 0;

        for (int i = 1; i < argc; i++) {
            const char *filename = argv[i];

            if (!strcmp(filename, STDIN_AS_FILENAME)) {
                read_from_stdin();
                continue;
            }

            int fd = open(filename, O_RDONLY);

            if (fd < 0) {
                print_opening_error(filename);
                continue;
            }

            valid_filenames++;
            if (argc > ARGC_TO_PRINT_NAMES) {
                if (valid_filenames >= ARGC_TO_PRINT_NAMES) {
                    write(STDOUT_FILENO, "\n", 1);
                }

                print_filename_as_title(filename);
            }

            tail_file(fd, filename);

            if (close(fd) < 0) {
                print_reading_error(filename);
                return -1;
            }
        }
    } else {
        read_from_stdin();
    }

    return 0;
}

//------------------------------------------------------------------------
// FUNCTION: print_filename_as_title
// Prints filename as a ==> title <===
// PARAMETERS:
// const char *filename -> the filename to be printed as a title
//------------------------------------------------------------------------
void print_filename_as_title(const char *filename) {
    write(STDOUT_FILENO, "==> ", 4);
    write(STDOUT_FILENO, filename, strlen(filename));
    write(STDOUT_FILENO, " <==\n", 5);
}

//------------------------------------------------------------------------
// FUNCTION: read_from_stdin
// Tails symbols from STDIN
// PARAMETERS: No parameters
//------------------------------------------------------------------------
void read_from_stdin() {
    offset_to_last_n_lines(STDIN_FILENO, LINES_TO_READ);
    cat_file(STDIN_FILENO, STDIN_AS_FILENAME);
}

//------------------------------------------------------------------------
// FUNCTION: offset_to_last_n_lines
// Offsets a given file descriptor to the last n given lines
// Using read() and lseek()
// PARAMETERS:
// int fd -> The file descriptor to be offsetted
// int n  -> Last <n> lines
//------------------------------------------------------------------------
void offset_to_last_n_lines(int fd, int n) {
    off_t pos = lseek(fd, 0, SEEK_END);
    char ch;
    int lines = 0;

    while (pos) {
        lseek(fd, --pos, SEEK_SET);
        read(fd, &ch, 1);

        if (ch == '\n') {
            if (lines++ == n) {
                break;
            }
        }
    }
}

//------------------------------------------------------------------------
// FUNCTION: cat_file
// Prints a given file descriptor content on STDIN
// PARAMETERS:
// int fd               -> The file descriptor
// const char *filename -> File's name
//------------------------------------------------------------------------
void cat_file(int fd, const char *filename) {
    char buffer[PIPE_BUF];
    ssize_t rresult, wresult;

    while((rresult = read(fd, buffer, PIPE_BUF)) != 0) {
        if (rresult < 0) {
            print_reading_error(filename);
            exit(-1);
        }

        wresult = 0;
        while (wresult != rresult) {
            ssize_t res = write(STDOUT_FILENO, buffer + wresult, rresult - wresult);

            if (wresult < 0) {
                print_writing_error(filename);
                exit(-1);
            }

            wresult += res;
        }
    }
}

//------------------------------------------------------------------------
// FUNCTION: tail_file
// Tails a given file (prints last 10 lines from it's content to STDIN)
// Using the already implemented functions
// offset_to_last_n_lines() and cat_file()
// PARAMETERS:
// int fd               -> The file descriptor
// const char *filename -> File's name
//------------------------------------------------------------------------
void tail_file(int fd, const char *filename) {
    offset_to_last_n_lines(fd, LINES_TO_READ);
    cat_file(fd, filename);
}
