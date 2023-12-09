#if !defined _INC_STDLIB
#include <stdlib.h>
#endif

#if !defined _INC_STRING
#include <string.h>
#endif

#if !defined __CLANG_LIMITS_H || !defined _GCC_LIMITS_H_
#include <limits.h>
#endif

#if !defined _INC_MYBASICS
#include "MyBasics.h"
#endif

// Determines the number of characters that would be within the string equivalent of INT_MAX.
#define INT_MAX_CHARS 11

/*
 * Writes to the first argument, str, from stream until reading the delimiter
 * character, or upon the string reaching the specified length.
 *
 * \returns Returns the length of the string INCLUDING the null terminator, or 0 if nothing was written.
 *
 * \note If this function returns 0, the string will be left unmodified (that is, nothing will be written).
 *
 * \note A null terminator will replace the delimiter within the string.
 *
 * \note Otherwise, if 'length' characters were written before the delimiter was read, a null terminator will be
 * appended to the end of the string if no delimiter was read or EOF was not met.
 *
 * \note If this occurs, the returned value will be length + 1.
 *
 * \param str The pointer to string that will be written to
 * \param delim The delimiter character
 * \param length The maximum size of the string
 * \param stream The input stream to read from
 */
size_t getStr(char **const str, const char delim, const size_t length, FILE *const stream)
{
    if (str == NULL)
    {
        fputs("\ngetStr(): Invalid pointer-to-pointer-to-char provided; No reading occurred\n", stderr);
        return 0;
    }

    if (max_length == 0)
    {
        fprintf(stderr, "\ngetStr(): Invalid string length (%llu); No reading occurred\n", max_length);
        return 0;
    }

    if (stream == NULL)
    {
        fputs("\ngetStr(): Invalid input stream provided; No reading occurred\n", stderr);
        return 0;
    }

    if (feof(stream))
        return 0;

    // "length + 1" to ensure there is always space for a null terminator
    char buffer[max_length + 1];
    size_t i = 0;
    for (; i < max_length; ++i)
    {
        buffer[i] = getc(stream);
        if (buffer[i] == delim || buffer[i] == EOF)
            break;
    }

    // If nothing meaningful was read then we can just return 0, leaving the 'str' argument untouched.
    // This will usually occur if only the specified delimiter character was read from the 'stream'
    // which gets replaced by the null terminator.
    if (buffer[0] == '\0')
        return 0;

    // Incrementing 'i' since we count the null terminator in the string's returned length
    buffer[i++] = '\0';

    // Allocating memory for 'str' if the user hadn't already allocated for it themselves
    if (*str == NULL)
    {
        *str = malloc(sizeof(buffer));
        if (*str == NULL)
        {
            fputs("\ngetStr(): malloc() failure; No reading occurred\n", stderr);
            return 0;
        }
    }
    strcpy_s(*str, sizeof(buffer), buffer);

    return i;
}

/*
 * Writes to the first argument, 'str', from stdin until reading a newline character,
 * or upon the string reaching the specified length.
 *
 * The difference between getStr() and this function is that this function has an
 * implicit delimiter, '\\n', an implicit stream, 'stdin', and will automatically flush
 * any unused input from stdin.
 *
 * This function has the same semantics as getStr() aside from the flushing of stdin.
 */
size_t getStrStdin(char **const str, const size_t length)
{
    const size_t numChars = getStr(str, '\n', length, stdin);
    // If 'numChars' is greater than the passed length, that means getStr() appended a null terminator
    // due to not encountering a newline, which means there is still input that needs to be flushed,
    // hence the fseek() call below.
    if (numChars > length)
        fseek(stdin, 0, SEEK_END);
    return numChars;
}

/*
 * Parses the passed string into an int which will be written to the second argument, 'num'.
 *
 * If this function fails, such as via invalid characters or overflow, 'num' will be left
 * unchanged and ERRCODE_GENERAL will be returned.
 */
int strToInt(const char *str, int *const num)
{
    if (str == NULL)
    {
        fputs("\nstrToInt(): 'str' is NULL; No conversion occurred", stderr);
        return ERRCODE_BAD_PTR;
    }

    if (num == NULL)
    {
        fputs("\nstrToInt(): 'num' is NULL; No conversion occurred", stderr);
        return ERRCODE_BAD_PTR;
    }

    const static unsigned MAX_PLACE_VALUE = 1000000000;

    const char containsSign = (*str == '-' || *str == '+');
    const char isNegative = (*str == '-');

    const char *const STR_PTR_START = str + containsSign;
    const char *const STR_PTR_END = str + strlen(str) - 1; // "strlen(str) - 1" to discount the null terminator

    if (STR_PTR_END - STR_PTR_START > INT_MAX_CHARS)
        return ERRCODE_BAD_INPUT;

    unsigned int tempNum = 0; // This will replace 'num' upon the function's successful completion
    unsigned int placeValue = 1;

    // Iterating over the string in reverse allows for the correct place values to be assigned with
    // as little hassle as possible
    for (str = STR_PTR_END; str >= STR_PTR_START; --str, placeValue *= (placeValue == MAX_PLACE_VALUE) ? 1 : 10)
    {
        const char currentDigit = charToInt(*str);

        if (currentDigit == -1)
            return ERRCODE_BAD_INPUT;

        // We can skip any values of 0
        if (currentDigit == 0 && placeValue < MAX_PLACE_VALUE)
            continue;

        tempNum += currentDigit * placeValue;

        if (tempNum >= (unsigned int)INT_MAX || tempNum / placeValue < 1)
        {
            if (tempNum == (unsigned int)INT_MAX)
            {
                break;
            }
            if (tempNum == (unsigned int)INT_MIN * -1 && isNegative)
            {
                *num = INT_MIN;
                return ERRCODE_SUCCESS;
            }
            return ERRCODE_BAD_INPUT;
        }
    }

    *num = tempNum;

    if (isNegative)
        *num *= -1;
    return ERRCODE_SUCCESS;
}

// Returns a random string of unsigned chars using random_vis_uchar().
unsigned char *random_ustring(const unsigned char min, const unsigned char max, const size_t length)
{
    if (max < min)
    {
        fprintf_s(stderr, "generate_test_ustring(): Invalid values for max and min values (%u is NOT GREATER THAN %u)\n", max, min);
        return NULL;
    }

    if (length == 0)
    {
        fprintf_s(stderr, "generate_test_ustring(): Invalid length: %llu", length);
        return NULL;
    }

    // (length + 1) to account for the null terminator
    unsigned char *str = malloc(length + 1);

    if (str == NULL)
    {
        fputs("generate_test_ustring(): malloc() failure; returning NULL\n", stderr);
        return NULL;
    }

    for (size_t i = 0; i < length; i++)
        str[i] = random_uchar_range(min, max);

    str[length] = '\0';

    return str;
}

// Returns a random string of signed chars using random_vis_uchar().
char *random_string(const char min, const char max, const size_t length)
{
    if (max < min)
    {
        fprintf_s(stderr, "generate_test_ustring(): Invalid values for max and min values (%u is NOT GREATER THAN %u)\n", max, min);
        return NULL;
    }

    // (length + 1) to account for the null terminator
    char *str = malloc(length + 1);
    if (str == NULL)
    {
        fputs("generate_test_string(): malloc() failure; returning NULL\n", stderr);
        return NULL;
    }

    for (size_t i = 0; i < length; i++)
        str[i] = random_uchar_range(min, max);

    str[length] = '\0';

    return str;
}

// Returns a random string of alphabetical chararacters using random_vis_uchar().
char *random_string_alphabetical(const size_t length)
{
    if (length == 0)
    {
        fprintf_s(stderr, "generate_test_string_alphabetic(): Invalid length: %llu", length);
        return 0;
    }

    // (length + 1) to account for the null terminator
    char *str = malloc(length + 1);
    if (str == NULL)
    {
        fprintf_s(stderr, "generate_test_string(): malloc() failure; returning NULL\n");
        return NULL;
    }

    for (size_t i = 0; i < length; i++)
    {
        if (random_bool())
            str[i] = random_uchar_range('a', 'z');
        else
            str[i] = random_uchar_range('A', 'Z');
    }
    str[length] = '\0';

    return str;
}

/*
 * Reads up to 11 numerical characters (including a preceding dash) and parses
 * them, writing the proper int equivalent (if any) to 'num'.
 *
 * Stops upon reading a non-numerical character
 *
 * Writes the parsed line as an int to 'num'.
 *
 * Returns 1 if the passed arguments were valid, but no characters in the stream
 * were parsable, or if type int was overflowed.
 */
int readInt(int *const num, FILE *stream)
{
    if (num == NULL)
    {
        fputs("\nreadInt(): Passed int pointer is NULL; No reading occurred", stderr);
        return ERRCODE_BAD_PTR;
    }

    if (stream == NULL)
    {
        fputs("\nreadInt(): Passed file is NULL; No reading occurred", stderr);
        return ERRCODE_BAD_FILE;
    }

    if (feof(stream))
    {
        fputs("\nreadInt(): Passed file is already at EOF; No reading occurred", stderr);
        return ERRCODE_FILE_AT_EOF;
    }

    // Buffer size of 12 (for Windows running on my Surface Go) since a typical int can hold 10 numbers
    // including a dash in the case of negative values for a total of 11 characters (INT_MAX_CHARS).
    // Then we need to account for the null terminator, so we add 1 to INT_MAX_CHARS.
    static char buffer[INT_MAX_CHARS + 1];

    int i = -1;
    // We count a single trailing dash as a valid character, so we increment 'i' just as we would for a digit
    if ((buffer[0] = getc(stream)) == '-')
        ++i;
    else
        ungetc(buffer[0], stream);

    while (i < INT_MAX_CHARS && isNumerical((buffer[++i] = getc(stream))))
        ;

    buffer[i] = '\0';

    return strToInt(buffer, num);
}
