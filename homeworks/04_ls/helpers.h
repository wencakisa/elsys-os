#ifndef HELPERS_H_
#define HELPERS_H_

#include <stdbool.h>
#include <sys/stat.h>
#include <sys/types.h>

bool contains(const char* haystack, const char* needle);

bool is_flag(const char* filename);

bool is_hidden(const char* filename);

bool is_parent_or_current_dir(const char* filename);

bool is_regular_file(mode_t mode);

bool is_directory(mode_t mode);

char get_mode_symbol(mode_t mode);

char* get_owner_name(struct stat file_stat);

char* get_group_name(struct stat file_stat);

char* join_paths(const char* parent, const char* child);

void print_error(const char* error_prefix, const char* filename);

#endif
