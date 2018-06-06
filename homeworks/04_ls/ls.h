#ifndef LS_H_
#define LS_H_

#include <stdlib.h>
#include <stdbool.h>
#include <dirent.h>
#include <sys/stat.h>

#define CURRENT_DIRECTORY "."

#define FULL_INFO_FLAG    "l"
#define SHOW_HIDDEN_FLAG  "A"
#define RECURSIVE_FLAG    "R"

typedef struct flags_t {
    bool print_title;
    bool full_info;
    bool show_hidden;
    bool recursive;
} flags_t;

// -----------------------------------------------------------------------------
// FUNCTION: init_flags
//     Acts as a constructor for a flags_t variable
// PARAMETERS:
//     flags_t* flags_ptr - Pointer to the flags to be initialized
// -----------------------------------------------------------------------------
void init_flags(flags_t* flags_ptr);

// -----------------------------------------------------------------------------
// FUNCTION: process_flag
//     Processes a given flag and setting a proper flag if present
// PARAMETERS:
//     flags_t* flags_ptr   - Flags to be changed
//     const char* flag_str - Flag string to be processed
// -----------------------------------------------------------------------------
void process_flag(flags_t* flags_ptr, const char* flag_str);

// -----------------------------------------------------------------------------
// FUNCTION: print_file_permissions
//     Prints file permissions as UNIX /bin/ls -l for a given mode
// PARAMETERS:
//     mode_t mode
// -----------------------------------------------------------------------------
void print_file_permissions(mode_t mode);

// -----------------------------------------------------------------------------
// FUNCTION: print_full_info
//     Prints full information (permissions, size, owners, etc.) for a given stat
// PARAMETERS:
//     struct stat file_stat
// -----------------------------------------------------------------------------
void print_full_info(struct stat file_stat);

// -----------------------------------------------------------------------------
// FUNCTION: get_total_blocks_size
//     Returns total blocks size for a directory as blkcnt_t
// PARAMETERS:
//     struct dirent** name_list - the list of files to be analyzed
//     int files_count           - the count of files
//     const char* base_filename - The parent filename
//     flags_t flags             - Given flags for the command
// -----------------------------------------------------------------------------
blkcnt_t get_total_blocks_size(struct dirent** name_list, int files_count, const char* base_filename, flags_t flags);

// -----------------------------------------------------------------------------
// FUNCTION: process_file
//     Processes a given file
// PARAMETERS:
//     struct stat file_stat
//     const char* filename
//     flags_t flags         - Given flags for the command
// -----------------------------------------------------------------------------
void process_file(struct stat file_stat, const char* filename, flags_t flags);

// -----------------------------------------------------------------------------
// FUNCTION: process_directory
//     Processes a given directory
// PARAMETERS:
//     const char* filename
//     flags_t flags         - Given flags for the command
// -----------------------------------------------------------------------------
void process_directory(const char* filename, flags_t flags);

// -----------------------------------------------------------------------------
// FUNCTION: process_directory
//     Processes CURRENT_DIRECTORY
// PARAMETERS:
//     flags_t flags - Given flags for the command
// -----------------------------------------------------------------------------
void process_current_directory(flags_t flags);

// -----------------------------------------------------------------------------
// FUNCTION: ls
//     Acts like UNIX /bin/ls command using the all already defined functions
// PARAMETERS:
//     int size                    - Files count
//     const char* filenames[size] - The files to be ls-ed :D
// -----------------------------------------------------------------------------
void ls(int size, const char* filenames[size]);

#endif
