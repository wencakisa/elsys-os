//------------------------------------------------------------------------
// FILE NAME: errors.c
// FILE PURPOSE:
// Provides more detailed error representation
// for opening, reading and writing files
//------------------------------------------------------------------------

#include <stdio.h>
#include <string.h>

#include "errors.h"

//------------------------------------------------------------------------
// FUNCTION: print_full_error
// Prints a fully described error message to STDERR
//
// PARAMETERS:
// const char *before_filename -> information to be printed before filename
// const char *filename        -> the filename that is causing the error
// const char *after_filename  -> information to be printed after filename
//------------------------------------------------------------------------
void print_full_error(const char *before_filename, const char *filename, const char *after_filename) {
    char error_message[MAX_ERR_MSG_LEN];
    strcpy(error_message, ERROR_BEGIN);
    strcat(error_message, before_filename);
    strcat(error_message, filename);
    strcat(error_message, after_filename);

    perror(error_message);
}

//------------------------------------------------------------------------
// FUNCTION: print_opening_error
// Prints a opening error to STDERR
//
// PARAMETERS:
// const char *filename -> the filename that is causing opening error
//------------------------------------------------------------------------
void print_opening_error(const char *filename) {
    print_full_error("cannot open '", filename, "' for reading");
}

//------------------------------------------------------------------------
// FUNCTION: print_reading_error
// Prints a reading error to STDERR
//
// PARAMETERS:
// const char *filename -> the filename that is causing reading error
//------------------------------------------------------------------------
void print_reading_error(const char *filename) {
    print_full_error("error reading '", filename, "'");
}

//------------------------------------------------------------------------
// FUNCTION: print_writing_error
// Prints a writing error to STDERR
//
// PARAMETERS:
// const char *filename -> the filename that is causing writing error
//------------------------------------------------------------------------
void print_writing_error(const char *filename) {
    print_full_error("error writing '", filename, "'");
}
