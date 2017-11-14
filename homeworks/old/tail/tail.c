#include <stdio.h>
#include <stdlib.h>

#define MAX_ERR_LEN  25
#define MAX_LINE_LEN 4096
#define TAIL_LINES   10

FILE* fopen_secure(const char*, const char*);
void print_lines(FILE*);
void print_last_n_lines(FILE*, int);
void tail_file(const char*, int);


int main(int argc, char const *argv[]) {
    for (size_t i = 1; i < argc; i++) {
        tail_file(argv[i], (argc > 2));
    }

    return 0;
}

FILE* fopen_secure(const char *filename, const char *modes) {
    char *error_message = calloc(MAX_ERR_LEN, sizeof(char));
    sprintf(error_message, "tail: cannot open `%s` for reading", filename);

    FILE *file = fopen(filename, modes);

    if (file == NULL) {
        perror(error_message);
        exit(EXIT_FAILURE);
    }

    free(error_message);

    return file;
}

void print_lines(FILE *file) {
    char line[MAX_LINE_LEN];

    while (fgets(line, sizeof(line), file) != NULL) {
        printf("%s", line);
    }
}

void print_last_n_lines(FILE *file, int n) {
    long int position;
    int count = 0;

    fseek(file, 0, SEEK_END);
    position = ftell(file);

    while (position) {
        fseek(file, --position, SEEK_SET);

        if (fgetc(file) == '\n') {
            if (count++ == TAIL_LINES) {
                break;
            }
        }
    }

    print_lines(file);
}

void tail_file(const char *filename, int print_filename) {
    FILE *file = fopen_secure(filename, "r");

    if (print_filename) {
        printf("==> %s <==\n", filename);
    }
    print_last_n_lines(file, TAIL_LINES);

    fclose(file);
}
