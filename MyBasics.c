#include <stdlib.h>
#include <string.h>
#include "MyBasics.h"

// Determines the maximum number of characters fDiscardLine() should discard before
// stopping if it doesn't reach a newline character.
#define FDISCARDLINE_MAX_READS 50000000

// Determines the number of characters that would be within the string equivalent of INT_MAX.
#define INT_MAX_CHARS 11

// Miscellaneous macros used by strToInt()
#define INT_MAX_PLACEVALUE 1000000000
#define INT_OVERFLOW_CHK 147483647
#define UINT_OVERFLOW_CHK (INT_OVERFLOW_CHK + 1)

/*
 * Will discard a line of input within 'stream' until meeting a newline character or EOF.
 * Note that this will also discard the newline character.
 *
 * Use fseek(stdin, 0, SEEK_END) to clear the contents of 'stdin' instead of using this function.
 *
 * To prevent potential hanging, the loop within this function will terminate after
 * reading an FDISCARDLINE_MAX_READS number of characters from 'stream'.
 *
 * Returns 0 on success, and 1 upon failure.
 */
inline int fDiscardLine(FILE *stream)
{
    for (int i = 0; i < FDISCARDLINE_MAX_READS; i++)
    {
        if (getc(stream) == '\n' || feof(stream))
            return ERRCODE_SUCCESS;
    }
    return ERRCODE_GENERAL;
}

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
 * \note Otherwise, if 'length' characters were written before the delimiter was read, a null terminator will be appended to the
 * end of the string if no delimiter was read or EOF was not met.
 *
 * \note If this occurs, the returned value will be length + 1.
 *
 * \param str The pointer to string that will be written to
 * \param delim The delimiter character
 * \param length The maximum size of the string
 * \param stream The input stream to read from
 */
size_t getStr(char **str, const char delim, const size_t length, FILE *stream)
{
    // Validating the pointer to the string
    if (str == NULL)
    {
        fputs("\ngetStr(): Invalid pointer-to-pointer-to-char provided; No reading occurred\n", stderr);
        return 0;
    }

    // Validating the length argument for the string
    if (length == 0)
    {
        fprintf(stderr, "\ngetStr(): Invalid string length (%llu); No reading occurred\n", length);
        return 0;
    }

    // Validating that the input stream is, well, valid
    if (stream == NULL)
    {
        fputs("\ngetStr(): Invalid stream provided; No reading occurred\n", stderr);
        return 0;
    }

    // Ensuring stream isn't at EOF here so that we don't go through all the other stuff below
    // just to return 0 anyway
    if (feof(stream))
        return 0;

    // Reading from stream into str
    // "length + 1" to ensure there is always space for a null terminator
    char buffer[length + 1];
    size_t i = 0;
    for (; i < length; i++)
    {
        buffer[i] = getc(stream);
        if (buffer[i] == delim || buffer[i] == EOF)
        {
            // Incrementing 'i' since we count the null terminator in the string's returned length
            buffer[i++] = '\0';
            break;
        }
    }

    // If nothing meaningful was read then we can just return 0, leaving the 'str' argument untouched.
    // This will usually occur if only the specified delimiter character was read from the 'stream'
    // which gets replaced by the null terminator.
    if (buffer[0] == '\0')
        return 0;

    // Appending a null terminator and adding it to the string's returned length, 'i', if not all of
    // the input within 'stream' was read
    if (i == length)
        buffer[i++] = '\0';

    // Allocating memory for 'str' if the user hadn't already allocated for it themselves
    if (*str == NULL)
    {
        *str = malloc(sizeof(buffer));
        // Allocation failure insurance
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
inline size_t getStrStdin(char **str, const size_t length)
{
    const size_t numChars = getStr(str, '\n', length, stdin);
    // If 'numChars' is greater than the passed length, that means getStr() appended a null terminator
    // due to not encountering a newline, which means there is still input that needs to be flushed,
    // hence the fseek() call below.
    if (numChars > length)
        fseek(stdin, 0, SEEK_END);
    return numChars;
}

/* Returns the first index of the passed character within the passed string after
 * (strlen(str) + offset).
 *
 * Returns -1 if the character is not present within the string.
 */
int indexOf(const char *const str, const char letter, const size_t offset)
{
    const size_t LENGTH_OF_STR = strlen(str);
    if (offset > LENGTH_OF_STR)
    {
        fprintf(stderr, "\nindexOf(): Invalid offset (%llu) for passed string; No reading occurred\n", offset);
        return -1;
    }

    for (size_t i = offset; i < LENGTH_OF_STR; i++)
    {
        if (str[i] == letter)
            return i;
    }
    return -1;
}

/* Returns the last index of the passed character within the passed string before
 * (strlen(str) - offset).
 *
 * Returns -1 if the character is not present within the string.
 */
int lastIndexOf(const char *const str, const char letter, const size_t offset)
{
    const size_t LENGTH_OF_STR = strlen(str);
    if (offset > LENGTH_OF_STR)
    {
        fprintf(stderr, "\nindexOf(): Invalid offset (%llu) for passed string; No reading occurred\n", offset);
        return -1;
    }

    for (size_t i = LENGTH_OF_STR - offset; i > 0; i--)
    {
        if (str[i] == letter)
            return i;
    }
    return -1;
}

/*
 * Returns true, if the passed char represents an alphabetical letter.
 *
 * Otherwise returns false.
 */
inline int isAlphabetical(const char letter)
{
    return ((letter >= 'a' && letter <= 'z') || (letter >= 'A' && letter <= 'Z'));
}

/* Returns non-zero, true, if the passed char represents a number.
 *
 * Otherwise, this returns 0, false.
 */
inline int isNumerical(const char number)
{
    return (number >= '0' && number <= '9');
}

/*
 * Returns non-zero, true, if the passed char represents either a number
 * or alphabetical letter.
 *
 * Returns 0, false, otherwise.
 */
inline int isAlphaNumerical(const char item)
{
    return (isAlphabetical(item) || isNumerical(item));
}

/*
 * Returns the passed character as an integer singleton (0-9), if possible.
 *
 * If the passed character does not represent an integer, this function returns -1.
 */
inline int charToInt(const char num)
{
    if (isNumerical(num))
        return num - '0';
    return -1;
}

/*
 * Parses the passed string into an int which will be written to the second argument, 'num'.
 *
 * If this function fails, such as via invalid characters or overflow, 'num' will be left
 * unchanged and ERRCODE_GENERAL will be returned.
 */
int strToInt(const char *str, int *const num)
{
    // Validating the string's existence
    if (str == NULL)
    {
        fputs("\nstrToInt(): Passed string is NULL; No conversion occurred", stderr);
        return ERRCODE_BAD_PTR;
    }

    // If there is a leading dash sign within the string then the later parsing loop will know
    // to ignore this character
    const char isNegative = (*str == '-');
    const char *const STR_PTR_START = str + isNegative;
    const char *const STR_PTR_END = STR_PTR_START + strlen(str);
    {
        const size_t STR_SIZE = STR_PTR_END - STR_PTR_START;
        // Preemptively handling a 0-length string or a single negative dash
        if (STR_SIZE == 0 || (STR_SIZE == 1 && isNegative))
            return ERRCODE_BAD_INPUT;

        // Removing any leading zeros and checking for invalid characters since the passed str should contain
        // only numerical characters, and any leading non-numerical characters would simply be ignored rather
        // than stopping function flow.
        // Otherwise, this would make things like "asd8327" valid, causing 'num' to equal 8327, but things like
        // "8327asd" would cause the function to terminate (as it should), leaving 'num' unchanged.
        while (str < STR_PTR_END && (*str == '0' || !isNumerical(*str)))
        {
            if (!isNumerical(*str))
                return ERRCODE_BAD_INPUT;
            str++;
        }

        // If a string of only zeros or similar is the passed string, we handle that here since it is technically
        // valid, but the above loop would set str to STR_PTR_END and the later parsing loop won't run, which would
        // then cause *num to be unchanged since the checks afterward would assume the input string was invalid.
        if (str == STR_PTR_END && *str - 1 == '0')
        {
            *num = 0;
            return ERRCODE_SUCCESS;
        }

        // Ensuring that the string isn't long enough to definitively overflow int
        if (STR_PTR_END - str > INT_MAX_CHARS)
        {
            fprintf(stderr, "\nstrToInt(): String too large (%u); No conversion occurred\n", STR_SIZE);
            return ERRCODE_BAD_INPUT;
        }
    }
    // This, assuming no issues arise while parsing the passed string, will replace the value at 'num'
    int tempNum = 0;
    int placeValue = 1;

    // Iterating over the string in reverse so we can assign the correct place values with as little hassle
    // as possible.
    // STR_PTR_END - 1 to discount the null terminator
    for (str = STR_PTR_END - 1; str >= STR_PTR_START; str--)
    {
        // This value will only hold a single digit's worth since it calls charToInt(), hence the
        // usage of 'char', however it could contain a negative value
        const char currentNum = charToInt(*str);

        // We can skip any values of 0
        if (currentNum == '0')
        {
            placeValue *= 10;
            continue;
        }
    
        if (currentNum != -1)
        {
            // Ensuring no overflow will occur
            if (tempNum - INT_OVERFLOW_CHK <= 0 && placeValue != INT_MAX_PLACEVALUE)
            {
                tempNum += currentNum * placeValue;
                placeValue *= 10;
            }
            // Safely handling anything that approaches INT_MAX or INT_MIN
            else if (currentNum <= 2)
            {
                // If the digits added to 'tempNum' so far are below INT_MAX (since 'tempNum' should never
                // be negative), there can be nothing else added without risking overflow, so we break out.
                // The number could still be negative however, so we let the function run to its end to
                // handle this.
                if (tempNum - INT_OVERFLOW_CHK <= 0)
                {
                    tempNum += currentNum * placeValue;
                    break;
                }
                // This just handles the edge case of INT_MIN being greater than INT_MAX by 1, which would
                // cause the above if-statement to not run (and int would overflow anyway if it did).
                // In that case, the value is almost definitely INT_MIN, so we just assign that to 'num'.
                if (isNegative && tempNum - UINT_OVERFLOW_CHK == 0)
                {
                    *num = INT_MIN;
                    return ERRCODE_SUCCESS;
                }
                // Catching overflow with an undefined return error code
                return ERRCODE_GENERAL;
            }
            else
            {
                return ERRCODE_BAD_INPUT;
            }
        }
        else // Break upon hitting anything non-numeric
        {
            break;
        }
    }

    // If 'str' is still in its initialized value then the loop must have hit an invalid character
    // upon the first iteration, so we leave 'num' alone as per the function's guarantee and exit
    if (STR_PTR_END - str == STR_PTR_END - STR_PTR_START - 1)
        return ERRCODE_GENERAL;

    if (isNegative)
        tempNum *= -1;

    *num = tempNum;
    return ERRCODE_SUCCESS;
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
int readInt(int *num, FILE *stream)
{
    // 'num' pointer validation
    if (num == NULL)
    {
        fputs("\nreadInt(): Passed int pointer is NULL; No reading occurred", stderr);
        return ERRCODE_BAD_PTR;
    }

    // 'stream' validation
    if (stream == NULL)
    {
        fputs("\nreadInt(): Passed file is NULL; No reading occurred", stderr);
        return ERRCODE_BAD_FILE;
    }

    // Preliminary check to ensure 'stream' isn't already at EOF
    if (feof(stream))
    {
        fputs("\nreadInt(): Passed file is already at EOF; No reading occurred", stderr);
        return ERRCODE_FILE_AT_EOF;
    }

    // Buffer size of 12 (for Windows running on my Surface Go) since a typical int can hold 10 numbers
    // including a dash in the case of negative values for a total of 11 characters (INT_MAX_CHARS).
    // Then we need to account for the null terminator, so we add 1 to INT_MAX_CHARS.
    static const unsigned short BUFFER_SIZE = sizeof(char) * (INT_MAX_CHARS + 1);
    char buffer[BUFFER_SIZE];

    int i = -1;
    // Handling the trailing dash if present
    if ((buffer[0] = getc(stream)) == '-')
    {
        ++i; // We count the dash as a valid character, so we increment 'i' just as we would for a digit
    }
    else // Restore the character otherwise since it could be a valid digit
    {
        ungetc(buffer[0], stream);
    }

    while (i < INT_MAX_CHARS && isNumerical((buffer[++i] = getc(stream))))
        ;

    // Setting the null terminator below
    buffer[i] = '\0';

    return strToInt(buffer, num);
}
