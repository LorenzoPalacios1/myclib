#ifndef _INC_STDIO
#include <stdio.h>
#endif

#ifndef _INC_MYBASICS
#define _INC_MYBASICS

#define ERRCODE_SUCCESS (0x0)     // Code denoting a successful execution.
#define ERRCODE_GENERAL (0x1)     // An undefined error code; consult the function's documentation.
#define ERRCODE_BAD_PTR (0x2)     // General code denoting a bad pointer argument.
#define ERRCODE_BAD_FILE (0x3)    // Code denoting a bad or NULL pointer to a FILE argument.
#define ERRCODE_FILE_AT_EOF (0x4) // Code denoting that the given FILE is at EOF *before* any reading occurred.
#define ERRCODE_BAD_INPUT (0x5)   // Code denoting a bad string (eg. passing alphabetical chars to strToInt())

/*
 * The minimum signed char value for a visible character (inclusive).
 */
#define VIS_CHAR_START (' ' + 1)
/*
 * The maximum signed char value for a visible character (inclusive).
 */
#define VIS_CHAR_END ('' - 1)

/*
 * Set to zero to prevent certain random generator methods from generating
 * a set of random data as needed and returning elements from the set.
 */
#define ALLOW_RANDOM_GEN_CACHING (1)

/*
 * Determines the number of elements (not bytes) that can be held in each cache.
 */
#if ALLOW_RANDOM_GEN_CACHING != 0
#define CACHE_SIZE (unsigned)(128)
#endif

size_t getStr(char **str, const char delim, const size_t length, FILE *stream);

size_t getStrStdin(char **str, const size_t length);

/*
 * Returns true, if the passed char represents an alphabetical letter.
 *
 * Otherwise returns false.
 */
static inline int isAlphabetical(const char letter)
{
    return ((letter >= 'a' && letter <= 'z') || (letter >= 'A' && letter <= 'Z'));
}

/*
 * Returns non-zero, true, if the passed char represents a number.
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

/*
 * Returns a single int from rand() within the specified range (inclusive).
 * Always returns 0 if (max < min).
 */
static inline int random_int(const int min, const int max)
{
    if (max < min)
    {
        fprintf(stderr, "random_int(): Invalid values for max and min values (%d is NOT GREATER THAN %d)\n", max, min);
        return 0;
    }
    return rand() % (max - min + 1) + min;
}

/*
 * Returns either 0 or 1.
 *
 * If caching is enabled, the first call to this function will stock the cache.
 */
static inline unsigned char random_bool(void)
{
#if (ALLOW_RANDOM_GEN_CACHING == 0)
    return rand() % 2;
#else
    static unsigned char cache[CACHE_SIZE];
    static unsigned int iterator = sizeof(cache);

    if (iterator == sizeof(cache))
    {
        for (unsigned i = 0; i < sizeof(cache); i++)
            cache[i] = rand() % 2;
        iterator = 0;
    }

    return cache[iterator++];
#endif
}

char *random_string(const char min, const char max, const size_t length);

char *random_string_alphabetical(const size_t length);

unsigned char *random_ustring(const unsigned char min, const unsigned char max, const size_t length);

/*
 * Returns a single visible unsigned char from rand()
 *
 * If caching is enabled, the first call to this function will stock the cache.
 */
static inline unsigned char random_vis_uchar(void)
{
#if (ALLOW_RANDOM_GEN_CACHING == 0)
    return random_int(VIS_CHAR_START, UCHAR_MAX);
#else
    static unsigned char cache[CACHE_SIZE];
    static unsigned int iterator = sizeof(cache);

    if (iterator == sizeof(cache))
    {
        for (unsigned i = 0; i < sizeof(cache); i++)
            cache[i] = random_int(VIS_CHAR_START, UCHAR_MAX);
        iterator = 0;
    }

    return cache[iterator++];
#endif
}

/*
 * Returns a single visible unsigned char from rand()
 *
 * If caching is enabled, the first call to this function will stock the cache.
 */
static inline unsigned char random_vis_char(void)
{
#if ALLOW_RANDOM_GEN_CACHING == 0
    return random_int(VIS_CHAR_START, CHAR_MAX);
#else
    static unsigned char cache[CACHE_SIZE];
    static unsigned int iterator = sizeof(cache);

    if (iterator == sizeof(cache))
    {
        for (unsigned i = 0; i < sizeof(cache); i++)
            cache[i] = random_int(VIS_CHAR_START, CHAR_MAX);
        iterator = 0;
    }

    return cache[iterator++];
#endif
}

/*
 * Returns a single unsigned char from rand() within the min and max values (inclusive)
 */
static inline unsigned char random_uchar_range(const unsigned char min, const unsigned char max)
{
    if (max < min)
    {
        fprintf(stderr, "random_uchar_range(): Invalid values for max and min values (%u is NOT GREATER THAN %u)\n", max, min);
        return 0;
    }
    return random_int(min, max);
}

int readInt(int *const num, FILE *stream);
#endif
