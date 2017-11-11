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

#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <limits.h>
#include <string.h>
// Error message representation functions
#include "errors.h"

#define LINES_TO_READ 10
#define ARGC_TO_PRINT_NAMES 2
#define STDIN_AS_FILENAME "-"
#define STDIN_TEMP_OUTPUT_FILENAME "stdin_temp.txt"

int is_stdin_filename(const char*);
void print_filename_as_title(const char*);
void offset_to_last_n_lines(int, int);
void cat_input_to_output(int, int, const char*);
void tail_descriptor(int, const char*);
int create_stdin_descriptor();
void tail_from_stdin();

int main(int argc, char const *argv[]) {
    if (argc > 1) {
        size_t valid_filenames = 0;

        for (int i = 1; i < argc; i++) {
            const char *filename = argv[i];

            if (is_stdin_filename(filename)) {
                tail_from_stdin();
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

            tail_descriptor(fd, filename);

            if (close(fd) < 0) {
                print_reading_error(filename);
                return -1;
            }
        }
    } else {
        tail_from_stdin();
    }

    return 0;
}

//------------------------------------------------------------------------
// FUNCTION: is_stdin_filename
// Returns whether the provided filename equals STDIN_AS_FILENAME
//
// PARAMETERS:
// const char *filename -> the filename
//------------------------------------------------------------------------
int is_stdin_filename(const char *filename) {
    return !strcmp(filename, STDIN_AS_FILENAME);
}

//------------------------------------------------------------------------
// FUNCTION: print_filename_as_title
// Prints filename as a ==> title <===
//
// PARAMETERS:
// const char *filename -> the filename to be printed as a title
//------------------------------------------------------------------------
void print_filename_as_title(const char *filename) {
    write(STDOUT_FILENO, "==> ", 4);
    write(STDOUT_FILENO, filename, strlen(filename));
    write(STDOUT_FILENO, " <==\n", 5);
}

//------------------------------------------------------------------------
// FUNCTION: offset_to_last_n_lines
// Offsets a given file descriptor to the last n given lines
// Using read() and lseek()
//
// PARAMETERS:
// int fd -> The file descriptor to be offsetted
// int n  -> Last <n> lines
//------------------------------------------------------------------------
void offset_to_last_n_lines(int fd, int n) {
    // At first, go to the end of the descriptor using lseek()
    off_t pos = lseek(fd, 0, SEEK_END);
    char ch;
    int lines = 0;

    while (pos) {
        // Read the current bit bit
        if (read(fd, &ch, 1) != 0) {
            // If the bit is a newline character -> increment lines
            if (ch == '\n') {
                if (lines++ == n) {
                    break;
                }
            }
        }

        // Move one bit backwards from the end
        lseek(fd, --pos, SEEK_SET);
    }
}

//------------------------------------------------------------------------
// FUNCTION: cat_input_to_output
// Prints a given input descriptor content into output descriptor
//
// PARAMETERS:
// int input            -> Input descriptor (read from)
// int output           -> Output descriptor (write to)
// const char *filename -> Input descriptor filename
//------------------------------------------------------------------------
void cat_input_to_output(int input, int output, const char *filename) {
    char buffer[PIPE_BUF];
    ssize_t rresult, wresult;

    while((rresult = read(input, buffer, PIPE_BUF)) != 0) {
        if (rresult < 0) {
            print_reading_error(filename);
            exit(-1);
        }

        wresult = 0;
        while (wresult != rresult) {
            ssize_t res = write(output, buffer + wresult, rresult - wresult);

            if (wresult < 0) {
                print_writing_error(filename);
                exit(-1);
            }

            wresult += res;
        }
    }
}

//------------------------------------------------------------------------
// FUNCTION: tail_descriptor
// Tails a given file (prints last 10 lines from it's content to STDIN)
// Using the already implemented functions
// offset_to_last_n_lines() and cat_input_to_output()
//
// PARAMETERS:
// int fd               -> The file descriptor
// const char *filename -> File's name
//------------------------------------------------------------------------
void tail_descriptor(int fd, const char *filename) {
    offset_to_last_n_lines(fd, LINES_TO_READ);
    cat_input_to_output(fd, STDOUT_FILENO, filename);
}

//------------------------------------------------------------------------
// FUNCTION: create_stdin_descriptor
// Creates and returns the file descriptor for STDIN
//
// IMPORTANT:
// O_RDWR | O_CREAT -> Creates and opens STDIN_TEMP_OUTPUT_FILENAME in Read-Write mode
// With the following permissions:
// S_IRUSR -> read permission, owner
// S_IRGRP -> read permission, group
// S_IROTH -> read permission, others
// (It's the same as chmod 444)
//
// Done by following this: https://stackoverflow.com/questions/2395465/create-a-file-in-linux-using-c
// Got permissions descriptions from: http://pubs.opengroup.org/onlinepubs/7908799/xsh/sysstat.h.html
//
// PARAMETERS: No parameters
//------------------------------------------------------------------------
int create_stdin_descriptor() {
    return open(STDIN_TEMP_OUTPUT_FILENAME, O_RDWR | O_CREAT, S_IRUSR | S_IRGRP | S_IROTH);
}

//------------------------------------------------------------------------
// FUNCTION: tail_from_stdin
// Tails symbols from STDIN
//
// PARAMETERS: No parameters
//------------------------------------------------------------------------
void tail_from_stdin() {
    int stdin_temp_fd = create_stdin_descriptor();

    // At first, read from STDIN and write to stdin_temp_fd
    cat_input_to_output(STDIN_FILENO, stdin_temp_fd, STDIN_TEMP_OUTPUT_FILENAME);
    // Then offset stdin_temp_fd to the last <LINES_TO_READ> lines
    offset_to_last_n_lines(stdin_temp_fd, LINES_TO_READ);
    // Finally, read from stdin_temp_fd and write to STDOUT
    cat_input_to_output(stdin_temp_fd, STDOUT_FILENO, STDIN_AS_FILENAME);

    // unlink() deletes a given filename, so we delete STDIN_TEMP_OUTPUT_FILENAME
    unlink(STDIN_TEMP_OUTPUT_FILENAME);
}
