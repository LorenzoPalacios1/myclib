#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#ifndef _INC_MYBASICS
#define _INC_MYBASICS

#define ERRCODE_SUCCESS (0x0) /* Denotes a successful execution. */
#define ERRCODE_GENERAL \
  (0x1) /* An undefined status code; consult the function's documentation. */
#define ERRCODE_BAD_PTR (0x2) /* Denotes a bad pointer argument. */
#define ERRCODE_BAD_FILE \
  (0x3) /* Denotes a bad or `NULL` pointer to a `FILE` argument. */
#define ERRCODE_FILE_AT_EOF \
  (0x4) /* The passed `FILE` is at `EOF` *before* any reading occurred. */
#define ERRCODE_BAD_INPUT                                                      \
  (0x5) /* Denotes a bad string (eg. passing alphabetical chars to strToInt()) \
         */
#define ERRCODE_FILE_REACHED_EOF \
  (0x6) /* The function reached `EOF` during execution. */

/* The minimum `signed char` value for a visible character (inclusive). */
#define VIS_CHAR_START (' ' + 1)
/* The maximum `signed char` value for a visible character (inclusive). */
#define VIS_CHAR_END ('' - 1)

/*
 * Set to `true` to prevent certain random generator methods from generating
 * a cache of random data as needed and returning elements from the cache.
 */
#define ALLOW_RANDOM_GEN_CACHING (true)

// Determines the number of elements (not bytes) that each function will cache.
#if (ALLOW_RANDOM_GEN_CACHING != 0)
#define CACHE_SIZE ((size_t)256)
#endif

/*
 * Writes to the first argument, str, from stream until reading the delimiter
 * character, or upon the string reaching the specified length.
 *
 * \returns Returns the length of the string discounting the null terminator, or
 * 0 if nothing was written.
 *
 * \note If this function returns 0, the string will be left unmodified (that
 * is, nothing will be written).
 *
 * \note A null terminator will replace the delimiter within the string.
 *
 * \note Otherwise, if 'length' characters were written before the delimiter was
 * read, a null terminator will be appended to the end of the string if no
 * delimiter was read or EOF was not met.
 *
 * \note If this occurs, the returned value will be length + 1.
 *
 * \param str The pointer to string that will be written to
 * \param delim The delimiter character
 * \param max_length The maximum size of the string
 * \param stream The input stream to read from
 */
size_t getStr(char *str, const char delim, const size_t max_length,
              FILE *const stream);

/*
 * Writes to the first argument, `str`, from `stdin` until reading a newline
 * character, or upon the string reaching the specified length.
 *
 * The difference between `getStr()` and this function is that this function has
 * an implicit delimiter, `\\n`, an implicit stream, `stdin`, and will
 * automatically flush any unused input from stdin.
 *
 * This function has the same semantics as `getStr()` otherwise.
 */
size_t getStrStdin(char *str, const size_t length);

/*
 * Returns `true` if the passed char represents an alphabetical letter.
 *
 * Otherwise returns `false`.
 */
inline bool isAlphabetical(const char letter) {
  return ((letter >= 'a' && letter <= 'z') || (letter >= 'A' && letter <= 'Z'));
}

/*
 * Returns `true` if the passed char represents a number.
 *
 * Otherwise returns `false`.
 */
inline bool isNumerical(const char number) {
  return (number >= '0' && number <= '9');
}

/*
 * Returns `true` if the passed char represents either a number or an
 * alphabetical letter.
 *
 * Otherwise returns `false`.
 */
inline bool isAlphaNumerical(const char item) {
  return (isAlphabetical(item) || isNumerical(item));
}

/*
 * Returns the passed character as an integer singleton (0-9), if possible.
 *
 * If the passed character does not represent an integer, this function returns
 * `-1`.
 */
inline int charToInt(const char num) {
  if (isNumerical(num)) return num - '0';
  return -1;
}

int strToInt(const char *const str, int *const num);

/*
 * Returns a single `int` from `rand()` within the specified range (inclusive).
 * Always returns `0` if (`max` < `min`).
 */
static inline int randomInt(const int min, const int max) {
  if (max < min) {
    fprintf(stderr,
            "randomInt(): Invalid values for max and min values (%d is NOT "
            "GREATER THAN %d)\n",
            max, min);
    return 0;
  }
  return rand() % (max - min + 1) + min;
}

/*
 * Returns either `true` or `false`.
 *
 * If caching is enabled, the first call to this function will stock the cache.
 */
bool randomBool(void) {
#if (ALLOW_RANDOM_GEN_CACHING == false)
  return rand() & 1;
#else
  /* For this particular cache, the stored elements are used as bitfields. */
  static uint_fast8_t cache[CACHE_SIZE];
  /* Setting the iterator to the end of the cache will prompt its restocking. */
  static size_t cache_iterator = CACHE_SIZE;
  static uint_fast8_t bit_iterator = 1;
  if (bit_iterator == (sizeof(*cache) * CHAR_BIT)) {
    bit_iterator = 1;
    cache_iterator++;
  }
  if (cache_iterator == CACHE_SIZE) {
    for (size_t i = 0; i < CACHE_SIZE; ++i) {
      cache[i] = rand();
    }
    cache_iterator = 0;
  }
  return cache[cache_iterator] & (1 << bit_iterator++);
#endif
}

char *randomString(const char min, const char max, const size_t length);

char *randomAlphabeticalString(const size_t length);

uint8_t *randomUnsignedString(const uint8_t min, const uint8_t max,
                              const size_t length);

/*
 * Returns a single visible `unsigned char` from `rand()`.
 *
 * If caching is enabled, the first call to this function will stock the cache.
 */
static inline unsigned char randomVisibleUnsignedChar(void) {
#if (ALLOW_RANDOM_GEN_CACHING == false)
  return randomInt(VIS_CHAR_START, UCHAR_MAX);
#else
  static unsigned char cache[CACHE_SIZE];
  static size_t iterator = CACHE_SIZE; /* Setting the iterator to the end of the
                                          cache will prompt its restocking. */

  if (iterator == CACHE_SIZE) {
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
static inline unsigned char randomVisibleChar(void) {
#if (ALLOW_RANDOM_GEN_CACHING == false)
  return randomInt(VIS_CHAR_START, CHAR_MAX);
#else
  static unsigned char cache[CACHE_SIZE];
  static size_t iterator = CACHE_SIZE; /* Setting the iterator to the end of the
                                          cache will prompt its restocking. */

  if (iterator == CACHE_SIZE) {
    for (size_t i = 0; i < CACHE_SIZE; i++)
      cache[i] = randomInt(VIS_CHAR_START, CHAR_MAX);
    iterator = 0;
  }

  return cache[iterator++];
#endif
}

/*
 * Returns a single `unsigned char` from `rand()` whose value is between `min`
 * and `max` (inclusive).
 */
static inline unsigned char randomUnsignedCharInRange(const unsigned char min,
                                                      const unsigned char max) {
  if (max < min) {
    fprintf(stderr,
            "random_uchar_range(): Invalid values for max and min values (%u "
            "is NOT GREATER THAN %u)\n",
            max, min);
    return 0;
  }
  return randomInt(min, max);
}

int readInt(int *const num, FILE *stream);
#endif
