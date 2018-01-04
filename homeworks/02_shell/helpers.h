//------------------------------------------------------------------------
// FILE NAME: helpers.h
// FILE PURPOSE:
// Helper functions like array manipulation used in the whole shell program
//------------------------------------------------------------------------

#ifndef HELPERS_H_
#define HELPERS_H_

#include <stdlib.h>

//------------------------------------------------------------------------
// FUNCTION: free_string_array
// Completely frees space for a dynamically allocated char** (array of strings)
// PARAMETERS:
// char** arr - The array to be freed
//------------------------------------------------------------------------
void free_string_array(char** arr);
//------------------------------------------------------------------------
// FUNCTION: count_char
// Counts a char encounters in a string
// PARAMETERS:
// const char* haystack - The string that we search in
// const char needle    - The char that we search for
//------------------------------------------------------------------------
size_t count_char(const char* haystack, const char needle);
//------------------------------------------------------------------------
// FUNCTION: split_into_array
// Splits a string by given delimiter into an array
// PARAMETERS:
// char* str             - The string to be splitted
// const char* delimiter - The delimiter to split by
//------------------------------------------------------------------------
char** split_into_array(char* str, const char* delimiter);

#endif /* end of include guard: HELPERS_H_ */
