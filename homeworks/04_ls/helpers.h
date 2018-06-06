#ifndef HELPERS_H_
#define HELPERS_H_

#include <stdbool.h>
#include <sys/stat.h>
#include <sys/types.h>

// -----------------------------------------------------------------------------
// FUNCTION: contains
//     Returns boolean, telling whether the first string contains the other one
// PARAMETERS:
//     const char* haystack - The string to be searched in
//     const char* needle   - The string to be searched for
// -----------------------------------------------------------------------------
bool contains(const char* haystack, const char* needle);

// -----------------------------------------------------------------------------
// FUNCTION: is_flag
//     Returns boolean, telling whether the given string is a flag
// PARAMETERS:
//     const char* str - The string to be checked
// -----------------------------------------------------------------------------
bool is_flag(const char* str);

// -----------------------------------------------------------------------------
// FUNCTION: is_hidden
//     Returns boolean, telling whether the given string is a hidden filename
// PARAMETERS:
//     const char* filename
// -----------------------------------------------------------------------------
bool is_hidden(const char* filename);

// -----------------------------------------------------------------------------
// FUNCTION: is_parent_or_current_dir
//     Returns boolean, telling whether the given string filename
//     is the parent or current directory ("." or "..")
// PARAMETERS:
//     const char* filename
// -----------------------------------------------------------------------------
bool is_parent_or_current_dir(const char* filename);

// -----------------------------------------------------------------------------
// FUNCTION: is_regular_file
//     Returns boolean, telling whether the given mode is for a regular file
// PARAMETERS:
//     mode_t mode
// -----------------------------------------------------------------------------
bool is_regular_file(mode_t mode);

// -----------------------------------------------------------------------------
// FUNCTION: is_directory
//     Returns boolean, telling whether the given mode is for a directory
// PARAMETERS:
//     mode_t mode
// -----------------------------------------------------------------------------
bool is_directory(mode_t mode);

// -----------------------------------------------------------------------------
// FUNCTION: get_mode_symbol
//     Returns a char, representing the POSIX symbol for a given mode
// PARAMETERS:
//     mode_t mode
// -----------------------------------------------------------------------------
char get_mode_symbol(mode_t mode);

// -----------------------------------------------------------------------------
// FUNCTION: get_owner_name
//     Extracts the owner name from a stat variable
// PARAMETERS:
//     struct stat file_stat
// -----------------------------------------------------------------------------
char* get_owner_name(struct stat file_stat);

// -----------------------------------------------------------------------------
// FUNCTION: get_group_name
//     Extracts the group name from a stat variable
// PARAMETERS:
//     struct stat file_stat
// -----------------------------------------------------------------------------
char* get_group_name(struct stat file_stat);

// -----------------------------------------------------------------------------
// FUNCTION: join_paths
//     Joins two paths to a single one
// PARAMETERS:
//     const char* parent - The parent directory
//     const char* child  - The child directory
// -----------------------------------------------------------------------------
char* join_paths(const char* parent, const char* child);

// -----------------------------------------------------------------------------
// FUNCTION: print_error
//     Prints an error message for a given filename with built-in perror() function
// PARAMETERS:
//     const char* error_prefix - The error origin
//     const char* filename     - The filename causing the error
// -----------------------------------------------------------------------------
void print_error(const char* error_prefix, const char* filename);

#endif
