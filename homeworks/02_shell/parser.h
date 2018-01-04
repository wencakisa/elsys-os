//------------------------------------------------------------------------
// FILE NAME: parser.h
// FILE PURPOSE:
// Functions parsing the read commands from stdin to be automatically
// used in exec*() functions for executing them
//------------------------------------------------------------------------

#ifndef PARSER_H_
#define PARSER_H_

#define PIPE_SYMBOL "|"
#define SPACE_DELIMITER " "

//------------------------------------------------------------------------
// FUNCTION: parse_pipes
// Splits a given string by PIPE_SYMBOL
// using the already implemented function split_into_array() in #helpers.h#
// PARAMETERS:
// char* cmdline - The line to be parsed
//------------------------------------------------------------------------
char** parse_pipes(char* cmdline);
//------------------------------------------------------------------------
// FUNCTION: parse_cmdline
// Parses a provided line from the input
// by tokenizing it by space and
// returning the tokens in an dynamically allocated array
// PARAMETERS:
// const char* cmdline - The line to be parsed
//------------------------------------------------------------------------
char** parse_cmdline(const char* cmdline);

#endif /* end of include guard: PARSER_H_ */
