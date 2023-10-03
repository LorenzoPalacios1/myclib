#ifndef _INC_STDIO
#include <stdio.h>
#endif

#define ERRCODE_SUCCESS 0     // Code denoting a successful execution.
#define ERRCODE_GENERAL 1     // An undefined error code; consult the function's documentation.
#define ERRCODE_BAD_PTR 2     // General code denoting a bad pointer argument.
#define ERRCODE_BAD_FILE 3    // Code denoting a bad or NULL pointer to a FILE argument.
#define ERRCODE_FILE_AT_EOF 4 // Code denoting that the given FILE is at EOF *before* any reading occurred.
#define ERRCODE_BAD_INPUT 5   // Code denoting a bad string (eg. passing alphabetical chars to strToInt())

size_t getStr(char **str, const char delim, const size_t length, FILE *stream);

size_t getStrStdin(char **str, const size_t length);

inline int isAlphabetical(const char letter)
{
    return ((letter >= 'a' && letter <= 'z') || (letter >= 'A' && letter <= 'Z'));
}

inline int isNumerical(const char number)
{
    return (number >= '0' && number <= '9');
}

inline int isAlphaNumerical(const char item)
{
    return (isAlphabetical(item) || isNumerical(item));
}

inline int charToInt(const char num)
{
    if (isNumerical(num))
        return num - '0';
    return -1;
}

int strToInt(const char *const str, int *const num);

int readInt(int *const num, FILE *stream);
