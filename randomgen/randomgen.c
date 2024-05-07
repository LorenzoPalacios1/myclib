#include "randomgen.h"

#include <limits.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

uint8_t *randomUnsignedString(const uint8_t min, const uint8_t max,
                              const size_t length) {
  if (max < min) {
    fprintf(stderr,
            "\ngenerate_test_ustring(): Invalid values for range"
            "(%u is NOT GREATER THAN %u)",
            max, min);
    return NULL;
  }
  if (length == 0) return NULL;

  /* (length + 1) to account for the null terminator. */
  uint8_t *str = malloc(length + 1);
  if (str == NULL) {
    fputs("\ngenerate_test_ustring(): malloc() failure; returning NULL",
          stderr);
    return NULL;
  }

  for (size_t i = 0; i < length; i++)
    str[i] = randomUnsignedCharInRange(min, max);

  str[length] = '\0';

  return str;
}

/* Returns a random string of signed chars using `random_vis_uchar()`. */
char *randomString(const char min, const char max, const size_t length) {
  if (max < min) {
    fprintf(stderr,
            "\ngenerate_test_ustring(): Invalid values for range (%u is NOT "
            "GREATER THAN %u)",
            max, min);
    return NULL;
  }

  // (length + 1) to account for the null terminator
  char *str = malloc(length + 1);
  if (str == NULL) {
    fputs("\ngenerate_test_string(): malloc() failure; returning NULL", stderr);
    return NULL;
  }

  for (size_t i = 0; i < length; i++)
    str[i] = randomUnsignedCharInRange(min, max);
  str[length] = '\0';

  return str;
}

char *randomAlphabeticalString(const size_t length) {
  if (length == 0) {
    fprintf(stderr, "\ngenerate_test_string_alphabetic(): Invalid length: %llu",
            length);
    return NULL;
  }

  /* (length + 1) to account for the null terminator */
  char *str = malloc(length + 1);
  if (str == NULL) {
    fputs("\ngenerate_test_string(): malloc() failure; returning NULL", stderr);
    return NULL;
  }

  for (size_t i = 0; i < length; i++) {
    if (randomBool() & 1)
      str[i] = randomUnsignedCharInRange('a', 'z');
    else
      str[i] = randomUnsignedCharInRange('A', 'Z');
  }
  str[length] = '\0';

  return str;
}

uint8_t randomVisibleChar(void) {
#if (!ALLOW_RANDOM_GEN_CACHING)
  return randomInt(VIS_CHAR_START, CHAR_MAX);
#else
  static uint8_t cache[CACHE_SIZE];
  static size_t iterator = CACHE_SIZE;

  if (iterator == CACHE_SIZE) {
    for (size_t i = 0; i < CACHE_SIZE; i++)
      cache[i] = randomInt(VIS_CHAR_START, CHAR_MAX);
    iterator = 0;
  }
  return cache[iterator++];
#endif
}

uint8_t randomUnsignedCharInRange(const uint8_t min, const uint8_t max) {
  if (max < min) {
    fprintf(stderr,
            "random_uchar_range(): Invalid values for max and min values (%u "
            "is NOT GREATER THAN %u)\n",
            max, min);
    return 0;
  }
  return randomInt(min, max);
}

uint8_t randomVisibleUnsignedChar(void) {
#if (!ALLOW_RANDOM_GEN_CACHING)
  return randomInt(VIS_CHAR_START, UCHAR_MAX);
#else
  static uint8_t cache[CACHE_SIZE];
  static size_t iterator = CACHE_SIZE;

  if (iterator == CACHE_SIZE) {
    for (size_t i = 0; i < CACHE_SIZE; i++)
      cache[i] = randomInt(VIS_CHAR_START, UCHAR_MAX);
    iterator = 0;
  }
  return cache[iterator++];
#endif
}

bool randomBool(void) {
#if (!ALLOW_RANDOM_GEN_CACHING)
  return rand() & 1;
#else
  /*
   * For this particular cache, the stored elements in the below array are used
   * as bitfields. The bits within each bitfield are accessed by shifting `0b1`
   * to the left `bit_iterator` times. Once `bit_iterator` reaches the maximum
   * number of bits within that bitfield, `cache_iterator` is incremented so
   * that the next bitfield can be used, and `bit_iterator` is reset to 0.
   */
  static uint8_t cache[CACHE_SIZE];
  /* Setting the iterator to the end of the cache will prompt its restocking. */
  static size_t cache_iterator = CACHE_SIZE;
  static uint8_t bit_iterator = 0;
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

int randomInt(const int min, const int max) {
  if (max < min) {
    fprintf(stderr,
            "randomInt(): Invalid values for max and min values (%d is NOT "
            "GREATER THAN %d)\n",
            max, min);
    return 0;
  }
  return rand() % (max - min + 1) + min;
}
