#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <time.h>
#include "MyBasics.h"


#define ERRCODE_SUCCESS 0     // Code denoting a successful execution.
#define ERRCODE_DEFAULT 1     // An undefined error code; consult the function's documentation.
#define ERRCODE_NULL_PTR 2    // General code denoting a bad or NULL primitive pointer argument (eg. char* or int*).
#define ERRCODE_NULL_FILE 3   // Code denoting a bad or NULL pointer to a FILE argument.
#define ERRCODE_FILE_AT_EOF 4 // Code denoting that the given FILE is at EOF *before* any reading occurred.
#define ERRCODE_BAD_STR 5

// Determines the maximum number of characters fDiscardLine() should discard before
// stopping if it doesn't reach a newline character.
#define FDISCARDLINE_MAX_READS 2000000

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
    return ERRCODE_DEFAULT;
}

/*
 * Writes to the first argument, str, from stream until reading the delimiter character, or upon the
 * string reaching the specified length.
 *
 * A null terminator will replace the delimiter within the string.
 *
 * Otherwise, if applicable and possible, a null terminator will be appended to the end of the string
 * if no delimiter was read or EOF was not met. If this occurs, the returned value will be length + 1.
 *
 * Returns the length of the string INCLUDING the null terminator, or -1 upon an error.
 * 
 * Returns 0 if stream is at EOF, or if the first character read is the specified delimiter.
 * If this function returns 0 or -1, 'str' WILL BE LEFT UNCHANGED (that is, nothing will be written).
 */
size_t getStr(char **str, const char delim, const size_t length, FILE *stream)
{
    // Validating the pointer to the string
    if (str == NULL)
    {
        fprintf(stderr, "\ngetStr(): Invalid pointer-to-pointer-to-char provided; No reading occurred\n");
        return -1;
    }

    // Validating the length argument for the string
    if (length <= 0)
    {
        fprintf(stderr, "\ngetStr(): Invalid string length: %lld; No reading occurred\n", length);
        return -1;
    }

    // Validating that the input stream is, well, valid
    if (stream == NULL)
    {
        fprintf(stderr, "\ngetStr(): Invalid stream provided; No reading occurred\n");
        return -1;
    }

    // Ensuring stream isn't at EOF here so that we don't go through all the other stuff below.
    // We just return 0 since there aren't any actual problems with the arguments; it's just that
    // the stream provided is at EOF, and having a separate return code for this could be useful.
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

    // If nothing was read, we can just return 0, leaving the 'str' argument untouched
    // This will usually occur if only the specified delimiter character was read from the 'stream'
    // which gets replaced by the null terminator
    if (buffer[0] == '\0')
    {
        return 0;
    }

    // Appending a null terminator and adding it to the string's returned length, 'i', if not all of
    // the input within 'stream' was read
    if (i == length)
    {
        buffer[i++] = '\0';
    }

    // Allocating memory for 'str' if the user hadn't already allocated for it themselves
    if (*str == NULL)
    {
        *str = malloc(sizeof(char) * i);
        // Allocation failure insurance
        if (*str == NULL)
        {
            fprintf(stderr, "\ngetStr(): malloc() failure; No reading occurred\n");
            return -1;
        }
    }
    strcpy_s(*str, sizeof(char) * i, buffer);

    return i;
}

/*
 * Writes to the first argument, str, from stdin until reading a newline character, or upon the
 * string reaching the specified length.
 *
 * The difference between getStr() and this function is that this function has an implicit delimiter,
 * '\\n', an implicit stream, 'stdin', and will automatically flush any unused input from stdin.
 *
 * A null terminator will replace the newline within the string.
 *
 * Otherwise, if the input exceeds the passed length, a null terminator will be APPENDED to the
 * end of the string if no newline was read or EOF was not met. If this occurs, the returned value
 * will be length + 1.
 *
 * Returns the length of the string INCLUDING the null terminator, or -1 upon an error.
 * Returns 0 if the first character read is a newline character.
 */
inline int getStrStdin(char **str, const size_t length)
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
 * offset.
 *
 * Returns -1 if the character is not present within the string.
 */
int indexOf(const char *str, const char letter, const size_t offset)
{
    const size_t LENGTH_OF_STR = strlen(str);
    // Ensuring offset is valid
    if (offset > LENGTH_OF_STR || offset < 0)
    {
        fprintf(stderr, "\nindexOf(): Invalid offset for passed string: %llu; No reading occurred\n", offset);
        return -1;
    }

    // Searching through the string for the character
    //
    char currentChar;
    for (size_t i = offset; i < LENGTH_OF_STR && (currentChar = str[i]) != '\0'; i++)
    {
        if (currentChar == letter)
        {
            return i;
        }
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
 * Returns non-zero, true, if the passed char represents either a number or
 * alphabetical letter.
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
inline short int charToInt(const char num)
{
    if (isNumerical(num))
        return num - '0';
    return -1;
}

/*
 * Parses the passed string into an int which will be written to the second argument, 'num'.
 *
 * If this function fails, such as via invalid characters or overflow, 'num' will be left unchanged
 * and an error code returned.
 */
int strToInt(const char *str, int *num)
{
    // Validating the string's existence
    if (str == NULL)
    {
        fprintf(stderr, "\nstrToInt(): Passed string is NULL; No conversion occurred\n");
        return ERRCODE_NULL_PTR;
    }

    // We can use shorts because the length of str should be no longer than INT_MAX_CHARS
    const unsigned short STR_SIZE = strlen(str);

    // The expression below checks if there is a leading dash sign within the string which tells the
    // later parsing loop to ignore this character
    // Also used as a boolean to convert the resultant int to a negative if applicable
    const unsigned short isNegative = (*str == '-');

    if (STR_SIZE == 0)
    {
        fprintf(stderr, "\nstrToInt(): Invalid string length: 0; No conversion occurred\n");
        return ERRCODE_BAD_STR; 
    }
    else if (STR_SIZE == 1 && isNegative) // Preemptively handling a single negative dash being passed
    {
        return ERRCODE_BAD_STR;
    }

    // Removing any leading zeros and checking for invalid characters since the passed str should contain
    // only numerical characters, and any leading non-numerical characters would simply be ignored rather
    // than stopping function flow.
    // Otherwise, this would make things like "asd8327" valid, causing 'num' to equal 8327, but things like
    // "8327asd" would cause the function to terminate, leaving 'num' unchanged.
    unsigned short endIndex = isNegative;
    while (endIndex < STR_SIZE && (str[endIndex] == '0' || !isNumerical(str[endIndex])))
    {
        if (!isNumerical(str[endIndex]))
            return ERRCODE_BAD_STR;
        endIndex++;
    }

    // If, for whatever reason, "-0000" or similar is the passed string, we handle that below since the
    // above loop would normally discard it
    if (endIndex == STR_SIZE && str[endIndex - 1] == '0')
    {
        *num = 0;
        return ERRCODE_SUCCESS;
    }

    // Checking to ensure that the string isn't long enough to definitively overflow int
    if (STR_SIZE - endIndex > INT_MAX_CHARS)
    {
        fprintf(stderr, "\nstrToInt(): Invalid string length: %u; No conversion occurred\n", STR_SIZE);
        return ERRCODE_BAD_STR;
    }

    // This, assuming no issues arise while parsing the passed string, will replace the value at 'num'
    int tempNum = 0;
    int placeValue = 1;

    // Iterating over 'str' in reverse so we can assign the correct place values with as little hassle
    // as possible.
    // "STR_SIZE - 1" to skip the null terminator
    short i = STR_SIZE - 1;
    for (; i >= endIndex; i--)
    {
        // This value will only hold a single digit's worth since it calls charToInt(), hence
        // "short", however it could contain a negative value
        const short currentNum = charToInt(str[i]);

        // We can skip over any values of 0
        if (currentNum == '0')
        {
            placeValue *= 10;
            continue;
        }

        if (currentNum != -1)
        {

            // Ensuring no overflow will occur
            if (tempNum - 147483647 <= 0 && placeValue != INT_MAX_PLACEVALUE)
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
                // Catching overflow
                return ERRCODE_DEFAULT;
            }
            else
            {
                return ERRCODE_BAD_STR;
            }
        }
        else // Breaking upon hitting anything non-numeric
        {
            break;
        }
    }

    // If 'i' is still its initialized value, that means the loop had hit an invalid character, so
    // we leave 'num' alone and return an error code
    if (i == STR_SIZE - 1)
        return ERRCODE_DEFAULT;

    // Finally, we handle the negative sign if necessary
    if (isNegative)
        tempNum *= -1;

    *num = tempNum;
    return ERRCODE_SUCCESS;
}

/*
 * Reads up to 11 numerical characters (including a preceding dash) and parses them, writing the proper int equivalent
 * (if any) to 'num'.
 * Stops upon reading a non-numerical character
 *
 * Writes the parsed line as an int to 'num'.
 *
 * Returns 1 if the passed arguments were valid, but no characters in the stream were parsable,
 * or if type int was overflowed.
 */
int readInt(int *num, FILE *stream)
{
    // 'num' pointer validation
    if (num == NULL)
    {
        fprintf(stderr, "\nreadInt(): Passed int pointer is NULL; No reading occurred\n");
        return ERRCODE_NULL_PTR;
    }

    // 'stream' validation
    if (stream == NULL)
    {
        fprintf(stderr, "\nreadInt(): Passed file is NULL; No reading occurred\n");
        return ERRCODE_NULL_FILE;
    }

    // Preliminary check to ensure 'stream' isn't already at EOF
    if (feof(stream))
    {
        fprintf(stderr, "\nreadInt(): Passed file is already at EOF; No reading occurred\n");
        return ERRCODE_FILE_AT_EOF;
    }

    // Buffer size of 12 (for Windows running on my Surface Go) since a typical int can hold 10 numbers
    // including a dash in the case of negative values for a total of 11 characters (INT_MAX_CHARS).
    // Then we need to account for the null terminator, so we add 1 to INT_MAX_CHARS.
    static const unsigned short BUFFER_SIZE = sizeof(char) * (INT_MAX_CHARS + 1);
    char buffer[BUFFER_SIZE];

    int i = -1;
    // Handling the trailing dash if present
    if ((buffer[0] = getc(stdin)) == '-')
    {
        ++i; // We count the dash as a valid character, so we increment 'i' just as we would for a digit
    }
    else // Restore the character otherwise since it could be a valid digit
    {
        ungetc(buffer[0], stdin);
    }

    while (i < INT_MAX_CHARS && isNumerical((buffer[++i] = getc(stream))))
        ;

    // Setting the null terminator below
    buffer[i] = '\0';

    return strToInt(buffer, num);
}
