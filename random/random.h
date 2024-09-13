/*
 * This file utilizes rand().
 * Ensure srand() is called with a valid seed before using these functions.
 */

#ifndef RANDOM_H
#define RANDOM_H

#include <stdbool.h>
#include <stddef.h>

#include "random.h"

/* The minimum `signed char` value for a visible ASCII character (inclusive). */
#define VIS_CHAR_START (' ' + 1)
/* The maximum `signed char` value for a visible ASCII character (inclusive). */
#define VIS_CHAR_END ('' - 1)

/*
 * Set to `true` for certain random generator methods to generate
 * a cache of data as needed and return elements from that cache.
 */
#define ALLOW_RANDOM_GEN_CACHING (true)

/*
 * Determines the number of elements (not bytes) each relevant function will
 * cache.
 */
#if (ALLOW_RANDOM_GEN_CACHING)
#define CACHE_SIZE ((size_t)1024)
#endif

/* Returns an `int` from `rand()` within the specified range (inclusive). */
int random_int(int min, int max);

/*
 * Returns either `true` or `false`.
 *
 * If caching is enabled, the first call to this function will create a cache,
 * then return the first element from the created cache.
 */
bool random_bool(void);

/* Returns a random string of `char` using `random_vis_uchar()`. */
char *random_raw_string(char min, char max, size_t length);

/*
 * Returns a random string of alphabetical chararacters using
 * `random_vis_uchar()`.
 */
char *random_alphabetical_raw_string(size_t length);

/* Returns a random string of `unsigned char` using `random_vis_uchar()`. */
unsigned char *random_unsigned_raw_string(unsigned char min, unsigned char max,
                                          size_t length);

/*
 * Returns a visible extended ASCII character using `rand()`.
 *
 * If caching is enabled, the first call to this function will stock the cache.
 */
unsigned char random_visible_unsigned_char(void);

/*
 * Returns a visible standard ASCII character using `rand()`.
 *
 * If caching is enabled, the first call to this function will stock the cache.
 */
char random_visible_char(void);

/*
 * Returns an `unsigned char` from `rand()` whose value is between `min`
 * and `max` (inclusive).
 */
unsigned char random_unsigned_char_in_range(unsigned char min,
                                            unsigned char max);

#endif
