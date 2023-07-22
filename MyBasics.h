#ifndef STDIO_H
#include <stdio.h>
#endif

#define ERRCODE_SUCCESS 0     // Code denoting a successful execution.
#define ERRCODE_DEFAULT 1     // An undefined error code; consult the function's documentation.
#define ERRCODE_NULL_PTR 2    // General code denoting a bad or NULL primitive pointer argument (eg. char* or int*).
#define ERRCODE_NULL_FILE 3   // Code denoting a bad or NULL pointer to a FILE argument.
#define ERRCODE_FILE_AT_EOF 4 // Code denoting that the given FILE is at EOF *before* any reading occurred.

// Miscellaneous macro(s)
#define NUM_FUNCTIONS 11

int fDiscardLine(FILE *stream);

size_t getStr(char **str, const char delim, const size_t length, FILE *stream);

int getStrStdin(char **str, const size_t length);

int indexOf(const char *str, const char letter, const size_t offset);

int isAlphabetical(const char letter);

int isAlphaNumerical(const char item);

int isNumerical(const char number);

short int charToInt(const char num);

int strToInt(const char *str, int *num);

int readInt(int *num, FILE *stream);
