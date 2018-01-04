#include <stdlib.h>
#include <string.h>

#include "parser.h"
#include "helpers.h"

char** parse_pipes(char* cmdline) {
    return split_into_array(cmdline, PIPE_SYMBOL);
}

char** parse_cmdline(const char* cmdline) {
    const size_t len = strlen(cmdline);

    // copy the provided constant cmdline
    // in order to split it with split_into_array()
    char* line_copy = (char*) calloc(sizeof(char), len + 1);
    strcpy(line_copy, cmdline);

    // split the string by SPACE_DELIMITER
    char** result = split_into_array(line_copy, SPACE_DELIMITER);

    // free the line copy, we do not need this anymore
    free(line_copy);

    return result;
}
