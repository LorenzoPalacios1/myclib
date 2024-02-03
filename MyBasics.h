#ifndef _INC_STDIO
#include <stdio.h>
#endif

#ifndef _INC_STDBOOL_H
#include <stdbool.h>
#endif

#ifndef _INC_MYBASICS
#define _INC_MYBASICS

#define ERRCODE_SUCCESS             (0x0) /* Denotes a successful execution.                                        */
#define ERRCODE_GENERAL             (0x1) /* An undefined status code; consult the function's documentation.        */
#define ERRCODE_BAD_PTR             (0x2) /* Denotes a bad pointer argument.                                        */
#define ERRCODE_BAD_FILE            (0x3) /* Denotes a bad or `NULL` pointer to a `FILE` argument.                  */
#define ERRCODE_FILE_AT_EOF         (0x4) /* The passed `FILE` is at `EOF` *before* any reading occurred.           */
#define ERRCODE_BAD_INPUT           (0x5) /* Denotes a bad string (eg. passing alphabetical chars to strToInt())    */
#define ERRCODE_FILE_REACHED_EOF    (0x6) /* The function reached `EOF` during execution.                           */

/* The minimum `signed char` value for a visible character (inclusive). */
#define VIS_CHAR_START (' ' + 1)
/* The maximum `signed char` value for a visible character (inclusive). */
#define VIS_CHAR_END ('' - 1)

/*
 * Set to `true` to prevent certain random generator methods from generating
 * a cache of random data as needed and returning elements from the cache.
 */
#define ALLOW_RANDOM_GEN_CACHING (true)

/* Determines the number of elements (not bytes) that each  cache. */
#if (ALLOW_RANDOM_GEN_CACHING != 0)
#define CACHE_SIZE ((size_t)256)
#endif

/*
 * Determines the maximum number of characters that would be within the string equivalent
 * of `INT_MAX`.
 */
#define INT_MAX_CHARS (11)

size_t getStr(char **const str, const char delim, const size_t max_length, FILE *const stream);

size_t getStrStdin(char **const str, const size_t length);

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
 * Returns true if the passed char represents either a number or an alphabetical letter.
 *
 * Returns false otherwise.
 */
static inline bool isAlphaNumerical(const char item)
{
    return (isAlphabetical(item) || isNumerical(item));
}

/*
 * Returns the passed character as an integer singleton (0-9), if possible.
 *
 * If the passed character does not represent an integer, this function returns `-1`.
 */
static inline int charToInt(const char num)
{
    if (isNumerical(num))
        return num - '0';
    return -1;
}

int strToInt(const char *const str, int *const num);

/*
 * Returns a single `int` from `rand()` within the specified range (inclusive).
 * Always returns `0` if (`max` < `min`).
 */
static inline int randomInt(const int min, const int max)
{
    if (max < min)
    {
        fprintf(stderr, "randomInt(): Invalid values for max and min values (%d is NOT GREATER THAN %d)\n", max, min);
        return 0;
    }
    return rand() % (max - min + 1) + min;
}

/*
 * Returns either `true` or `false`.
 *
 * If caching is enabled, the first call to this function will stock the cache.
 */
static inline unsigned char randomBool(void)
{
#if (ALLOW_RANDOM_GEN_CACHING == false)
    return rand() & 1;
#else
    static unsigned char cache[CACHE_SIZE];
    /* Setting the iterator to the end of the cache will prompt its restocking. */
    static size_t iterator = CACHE_SIZE;

    if (iterator == CACHE_SIZE)
    {
        for (size_t i = 0; i < CACHE_SIZE; i++)
            cache[i] = rand() & 1;
        iterator = 0;
    }

    return cache[iterator++];
#endif
}

char *randomString(const char min, const char max, const size_t length);

char *randomAlphabeticalString(const size_t length);

unsigned char *randomUnsignedString(const unsigned char min, const unsigned char max, const size_t length);

/*
 * Returns a single visible `unsigned char` from `rand()`.
 *
 * If caching is enabled, the first call to this function will stock the cache.
 */
static inline unsigned char randomVisibleUnsignedChar(void)
{
#if (ALLOW_RANDOM_GEN_CACHING == false)
    return randomInt(VIS_CHAR_START, UCHAR_MAX);
#else
    static unsigned char cache[CACHE_SIZE];
    /* Setting the iterator to the end of the cache will prompt its restocking. */
    static size_t iterator = CACHE_SIZE;

    if (iterator == CACHE_SIZE)
    {
        for (size_t i = 0; i < CACHE_SIZE; i++)
            cache[i] = randomInt(VIS_CHAR_START, UCHAR_MAX);
        iterator = 0;
    }

    return cache[iterator++];
#endif
}

/*
 * Returns a single visible `signed char` from `rand()`.
 *
 * If caching is enabled, the first call to this function will stock the cache.
 */
static inline unsigned char randomVisibleChar(void)
{
#if (ALLOW_RANDOM_GEN_CACHING == false)
    return randomInt(VIS_CHAR_START, CHAR_MAX);
#else
    static unsigned char cache[CACHE_SIZE];
    static size_t iterator = CACHE_SIZE;

    if (iterator == CACHE_SIZE)
    {
        for (size_t i = 0; i < CACHE_SIZE; i++)
            cache[i] = randomInt(VIS_CHAR_START, CHAR_MAX);
        iterator = 0;
    }

    return cache[iterator++];
#endif
}

/*
 * Returns a single `unsigned char` from `rand()` whose value is between `min` and `max` (inclusive).
 */
static inline unsigned char randomUnsignedCharInRange(const unsigned char min, const unsigned char max)
{
    if (max < min)
    {
        fprintf(stderr, "random_uchar_range(): Invalid values for max and min values (%u is NOT GREATER THAN %u)\n",
                max, min);
        return 0;
    }
    return randomInt(min, max);
}

int readInt(int *const num, FILE *stream);
#endif
