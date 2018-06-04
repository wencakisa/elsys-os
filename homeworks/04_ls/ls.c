#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <errno.h>
#include <string.h>
#include <limits.h>
#include <time.h>
#include <sys/stat.h>

#include "helpers.h"
#include "ls.h"

void init_flags(flags_t* flags_ptr) {
    flags_ptr->print_title = false;
    flags_ptr->full_info   = false;
    flags_ptr->show_hidden = false;
    flags_ptr->recursive   = false;
}

void process_flag(flags_t* flags_ptr, const char* flag_str) {
    flags_ptr->full_info   = contains(flag_str, FULL_INFO_FLAG);
    flags_ptr->show_hidden = contains(flag_str, SHOW_HIDDEN_FLAG);
    flags_ptr->recursive   = contains(flag_str, RECURSIVE_FLAG);
}

void print_file_permissions(mode_t mode) {
    printf((mode & S_IRUSR) ? "r" : "-");
    printf((mode & S_IWUSR) ? "w" : "-");
    printf((mode & S_IXUSR) ? "x" : "-");
    printf((mode & S_IRGRP) ? "r" : "-");
    printf((mode & S_IWGRP) ? "w" : "-");
    printf((mode & S_IXGRP) ? "x" : "-");
    printf((mode & S_IROTH) ? "r" : "-");
    printf((mode & S_IWOTH) ? "w" : "-");
    printf((mode & S_IXOTH) ? "x" : "-");
}

void print_full_info(struct stat file_stat) {
    print_file_permissions(file_stat.st_mode);

    char timestamp[20];
    strftime(timestamp, 20, "%B %d %R", localtime(&file_stat.st_mtime));

    printf(" %ld %s %s %ld %s", file_stat.st_nlink,
                                get_owner_name(file_stat),
                                get_group_name(file_stat),
                                file_stat.st_size,
                                timestamp);
}

blkcnt_t get_total_blocks_size(struct dirent** name_list, int files_count, const char* base_filename, flags_t flags) {
    blkcnt_t total_blocks_size = 0;

    for (size_t i = 0; i < files_count; i++) {
        const char* current_name = name_list[i]->d_name;

        char* full_path = join_paths(base_filename, current_name);
        struct stat current_file_stat;
        stat(full_path, &current_file_stat);

        free(full_path);

        if (
            (!flags.show_hidden && is_hidden(current_name)) ||
            is_parent_or_current_dir(current_name)
        ) {
            continue;
        }

        total_blocks_size += current_file_stat.st_blocks;

    }

    return total_blocks_size / 2;
}

void process_file(struct stat file_stat, const char* filename, flags_t flags) {
    if (
        (!flags.show_hidden && is_hidden(filename)) ||
        is_parent_or_current_dir(filename)
    ) {
        return;
    }

    printf("%c", get_mode_symbol(file_stat.st_mode));

    if (flags.full_info) {
        print_full_info(file_stat);
    }

    printf(" %s\n", filename);
}

void process_directory(const char* filename, flags_t flags) {
    struct dirent** name_list;
    int files_count = scandir(filename, &name_list, NULL, alphasort);

    char** directories = (char**) malloc(sizeof(char*) * files_count + 1);
    size_t directories_count = 0;

    blkcnt_t total_blocks_count;
    if (flags.full_info) {
        total_blocks_count = get_total_blocks_size(name_list, files_count, filename, flags);
    }

    if (files_count < 0)
        print_error("ls: cannot open directory %s/", filename);
    else {
        if (flags.print_title) {
            printf("%s:\n", filename);
        }

        if (flags.full_info) {
            printf("total %ld\n", total_blocks_count);
        }

        for (size_t i = 0; i < files_count; i++) {
            const char* current_name = name_list[i]->d_name;

            char* full_path = join_paths(filename, current_name);
            struct stat current_file_stat;
            stat(full_path, &current_file_stat);

            process_file(current_file_stat, current_name, flags);

            bool is_directory_to_process = is_directory(current_file_stat.st_mode) &&
                                           flags.recursive &&
                                           !is_parent_or_current_dir(current_name);

            if (!flags.show_hidden) {
                is_directory_to_process = is_directory_to_process && !is_hidden(current_name);
            }

            if (is_directory_to_process) {
                directories[directories_count++] = full_path;
            } else {
                free(full_path);
            }

            free(name_list[i]);
        }

        flags.print_title = true;
        for(size_t i = 0; i < directories_count; i++) {
            printf("\n");
            process_directory(directories[i], flags);
            free(directories[i]);
        }

        free(directories);
        free(name_list);
    }
}

void process_current_directory(flags_t flags) {
    process_directory(CURRENT_DIRECTORY, flags);
}

void ls(int size, const char* filenames[size]) {
    flags_t flags;
    init_flags(&flags);

    if (size == 0) {
        flags.print_title = false;
        process_current_directory(flags);
    }

    struct stat file_stat;

    int flags_count = 0;

    for (size_t i = 0; i < size; i++) {
        const char* filename = filenames[i];

        if (is_flag(filename)) {
            flags_count++;
            process_flag(&flags, filename);

            if (size == 1) {
                flags.print_title = flags.recursive;
                process_current_directory(flags);
            }
        } else {
            mode_t last_processed_mode;

            if (stat(filename, &file_stat) == 0) {
                mode_t mode = file_stat.st_mode;

                if (is_directory(mode)) {
                    flags.print_title = (size - flags_count > 1);

                    if (is_regular_file(last_processed_mode)) {
                        printf("\n");
                    }

                    process_directory(filename, flags);

                    if (flags.print_title && i < size - 1) {
                        printf("\n");
                    }
                } else {
                    process_file(file_stat, filename, flags);
                }

                last_processed_mode = mode;
            } else {
                print_error("ls: cannot access %s", filename);
            }
        }
    }
}
