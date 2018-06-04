#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <ctype.h>
#include <sys/stat.h>
#include <pwd.h>
#include <grp.h>

#include "helpers.h"

bool contains(const char* haystack, const char* needle) {
    return strstr(haystack, needle) != NULL;
}

bool is_flag(const char* filename) {
    return filename[0] == '-' && strlen(filename) > 1;
}

bool is_hidden(const char* filename) {
    return filename[0] == '.' || filename[strlen(filename) - 1] == '~';
}

bool is_parent_or_current_dir(const char* filename) {
    return !strcmp(filename, ".") || !strcmp(filename, "..");
}

bool is_regular_file(mode_t mode) {
    return S_ISREG(mode) != 0;
}

bool is_directory(mode_t mode) {
    return S_ISDIR(mode) != 0;
}

char get_mode_symbol(mode_t mode) {
    if (is_regular_file(mode)) {
        return '-';
    } else if (is_directory(mode)) {
        return 'd';
    } else if (S_ISCHR(mode)) {
        return 'c';
    } else if (S_ISBLK(mode)) {
        return 'b';
    } else if (S_ISFIFO(mode)) {
        return 'p';
    } else if (S_ISLNK(mode)) {
        return 'l';
    } else if (S_ISSOCK(mode)) {
        return 's';
    }
}

char* get_owner_name(struct stat file_stat) {
    return getpwuid(file_stat.st_uid)->pw_name;
}

char* get_group_name(struct stat file_stat) {
    return getgrgid(file_stat.st_gid)->gr_name;
}

char* join_paths(const char* parent, const char* child) {
    char* path = (char*) malloc(strlen(parent) + strlen(child) + 2);
    int path_length = snprintf(path, PATH_MAX, "%s/%s", parent, child);

    if (path_length >= PATH_MAX) {
        perror("ls: path length has gone too long");
        exit(EXIT_FAILURE);
    }

    return path;
}

void print_error(const char* error_prefix, const char* filename) {
    const size_t error_message_length = strlen(error_prefix) + strlen(filename);
    char* error_message = (char*) malloc(error_message_length * sizeof(char));
    sprintf(error_message, error_prefix, filename);
    perror(error_message);
    free(error_message);
}
