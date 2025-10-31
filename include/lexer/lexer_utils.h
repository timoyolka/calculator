#ifndef LEXER_UTILS_H
#define LEXER_UTILS_H

#include <stdlib.h>
#include <string.h>
#include <stddef.h>

char *empty_str(); //Mallocs an empty string, just "\0"
size_t count_non_delim(const char *str, size_t len, char delim); //Counts the charachters without the delimiter
char* trim_char(const char *to_trim, size_t len, char delim);  //Returns new trimmed string
char* char_to_string(char c); //Converts a single char to a string with one charachter

#endif
