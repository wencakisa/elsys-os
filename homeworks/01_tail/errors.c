//------------------------------------------------------------------------
// FILE NAME: errors.c
// FILE PURPOSE:
// Provides more detailed error representation
// for opening, reading and writing files
//------------------------------------------------------------------------

#include <stdio.h>
#include <string.h>

#include "errors.h"

#define MAX_ERR_MSG_LEN 1024
#define ERROR_BEGIN "tail:"

//------------------------------------------------------------------------
// FUNCTION: print_full_error
// Prints a fully described error message to STDERR
//
// PARAMETERS:
// const char *error_message -> the concrete error message
//------------------------------------------------------------------------
void print_full_error(const char *error_message) {
    char full_error_message[MAX_ERR_MSG_LEN];
    sprintf(full_error_message, "%s %s", ERROR_BEGIN, error_message);

    perror(full_error_message);
}

//------------------------------------------------------------------------
// FUNCTION: print_opening_error
// Prints a opening error to STDERR
//
// PARAMETERS:
// const char *filename -> the filename that is causing opening error
//------------------------------------------------------------------------
void print_opening_error(const char *filename) {
    char error_message[MAX_ERR_MSG_LEN];
    sprintf(error_message, "cannot open '%s' for reading", filename);

    print_full_error(error_message);
}

//------------------------------------------------------------------------
// FUNCTION: print_io_error
// Prints either reading or writing error to STDERR
//
// PARAMETERS:
// const char *operation - "reading" / "writing"
// const char *filename  - the filename that is causing the IO error
//------------------------------------------------------------------------
void print_io_error(const char *operation, const char *filename) {
    char error_message[MAX_ERR_MSG_LEN];
    sprintf(error_message, "error %s '%s'", operation, filename);

    print_full_error(error_message);
}

//------------------------------------------------------------------------
// FUNCTION: print_reading_error
// Prints a reading error to STDERR
//
// PARAMETERS:
// const char *filename -> the filename that is causing reading error
//------------------------------------------------------------------------
void print_reading_error(const char *filename) {
    print_io_error("reading", filename);
}

//------------------------------------------------------------------------
// FUNCTION: print_writing_error
// Prints a writing error to STDERR
//
// PARAMETERS:
// const char *filename -> the filename that is causing writing error
//------------------------------------------------------------------------
void print_writing_error(const char *filename) {
    print_io_error("writing", filename);
}
