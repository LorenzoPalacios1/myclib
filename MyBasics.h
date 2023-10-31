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

void *lsearch_mid(const void *const needle, const void *const haystack, const size_t haystack_size, int cmp(const void *const a, void const *const b));

/*
 * Returns true, if the passed char represents an alphabetical letter.
 *
 * Otherwise returns false.
 */
static inline int isAlphabetical(const char letter)
{
    return ((letter >= 'a' && letter <= 'z') || (letter >= 'A' && letter <= 'Z'));
}

/* Returns non-zero, true, if the passed char represents a number.
 *
 * Otherwise, this returns 0, false.
 */
static inline int isNumerical(const char number)
{
    return (number >= '0' && number <= '9');
}

/*
 * Returns non-zero, true, if the passed char represents either a number
 * or alphabetical letter.
 *
 * Returns 0, false, otherwise.
 */
static inline int isAlphaNumerical(const char item)
{
    return (isAlphabetical(item) || isNumerical(item));
}

/*
 * Returns the passed character as an integer singleton (0-9), if possible.
 *
 * If the passed character does not represent an integer, this function returns -1.
 */
static inline int charToInt(const char num)
{
    if (isNumerical(num))
        return num - '0';
    return -1;
}

int strToInt(const char *const str, int *const num);

int readInt(int *const num, FILE *stream);
