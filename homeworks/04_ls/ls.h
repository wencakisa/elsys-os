#ifndef LS_H_
#define LS_H_

#include <stdlib.h>
#include <stdbool.h>
#include <dirent.h>
#include <sys/stat.h>

#define CURRENT_DIRECTORY "."

#define FULL_INFO_FLAG    "l"
#define SHOW_HIDDEN_FLAG  "a"
#define RECURSIVE_FLAG    "R"

typedef struct flags_t {
    bool print_title;
    bool full_info;
    bool show_hidden;
    bool recursive;
} flags_t;

void init_flags(flags_t* flags_ptr);

void process_flag(flags_t* flags_ptr, const char* flag_str);

void print_file_permissions(mode_t mode);

void print_full_info(struct stat file_stat);

blkcnt_t get_total_blocks_size(struct dirent** name_list, int files_count, const char* base_filename, flags_t flags);

void process_file(struct stat file_stat, const char* filename, flags_t flags);

void process_directory(const char* filename, flags_t flags);

void process_current_directory(flags_t flags);

void ls(int size, const char* filenames[size]);

#endif
