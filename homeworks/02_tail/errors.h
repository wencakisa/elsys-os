#ifndef ERRORS_H_
#define ERRORS_H_

#define ERROR_BEGIN "tail: "
#define MAX_ERR_MSG_LEN 1024

void print_full_error(const char*, const char*, const char*);
void print_opening_error(const char*);
void print_reading_error(const char*);
void print_writing_error(const char*);

#endif
