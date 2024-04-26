#ifndef RANDOMGEN_H
#define RANDOMGEN_H

#include <limits.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "randomgen.h"

/* The minimum `signed char` value for a visible character (inclusive). */
#define VIS_CHAR_START (' ' + 1)
/* The maximum `signed char` value for a visible character (inclusive). */
#define VIS_CHAR_END ('' - 1)

/*
 * Set to `true` to prevent certain random generator methods from generating
 * a cache of random data as needed and returning elements from the cache.
 *
 * Note that these caches may rely on the usage of rand(), so ensure that at
 * some point prior to calling these functions srand() is called.
 */
#define ALLOW_RANDOM_GEN_CACHING (true)

/*
 * Determines the number of elements (not bytes) that each function will cache.
 * Feel free to change the cache size as needed.
 */
#if (ALLOW_RANDOM_GEN_CACHING)
#define CACHE_SIZE ((size_t)256)
#endif

/*
 * Returns a single `int` from `rand()` within the specified range (inclusive).
 * Always returns `0` if (`max` < `min`).
 */
static inline int randomInt(const int min, const int max);

/*
 * Returns either `true` or `false`.
 *
 * If caching is enabled, the first call to this function will stock the cache.
 */
bool randomBool(void);

/* Returns a random string of signed chars using `random_vis_uchar()`. */
char *randomString(const char min, const char max, const size_t length);

/*
 * Returns a random string of alphabetical chararacters using
 * random_vis_uchar().
 */
char *randomAlphabeticalString(const size_t length);

/* Returns a random string of `unsigned char` using `random_vis_uchar()`. */
uint8_t *randomUnsignedString(const uint8_t min, const uint8_t max,
                              const size_t length);

/*
 * Returns a single visible `unsigned char` from `rand()`.
 *
 * If caching is enabled, the first call to this function will stock the cache.
 */
static inline unsigned char randomVisibleUnsignedChar(void);

/*
 * Returns a single visible `signed char` from `rand()`.
 *
 * If caching is enabled, the first call to this function will stock the cache.
 */
static inline unsigned char randomVisibleChar(void);

/*
 * Returns a single `unsigned char` from `rand()` whose value is between `min`
 * and `max` (inclusive).
 */
static inline unsigned char randomUnsignedCharInRange(const unsigned char min,
                                                      const unsigned char max);

#endif
