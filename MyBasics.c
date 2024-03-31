#include "MyBasics.h"

#include <limits.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

size_t getStr(char *str, const char delim, const size_t max_length,
              FILE *const stream) {
  if (str == NULL) {
    fputs("\ngetStr(): Invalid string pointer; No reading occurred", stderr);
    return 0;
  }
  if (stream == NULL) {
    fputs("\ngetStr(): Invalid input stream; No reading occurred", stderr);
    return 0;
  }
  if (max_length == 0) return 0;
  if (feof(stream)) return 0;

  /* `max_length + 1` to ensure there is always space for a null terminator */
  if (str == NULL) {
    str = malloc(max_length + 1);
    if (str == NULL) {
      fputs("\ngetStr(): malloc() failure; No writing occurred", stderr);
      return 0;
    }
  }
  size_t i = 0;
  for (; i < max_length; ++i) {
    str[i] = getc(stream);
    if (str[i] == delim || str[i] == EOF) break;
  }
  str[i] = '\0';

  return i;
}

size_t getStrStdin(char *str, const size_t max_length) {
  const size_t str_len = getStr(str, '\n', max_length, stdin);
  /* Discard any unused input. */
  if (str_len > max_length)
    while (getchar() != '\n')
      ;
  return str_len;
}

/* Returns a random string of `unsigned char` using `random_vis_uchar()`. */
unsigned char *randomUnsignedString(const unsigned char min,
                                    const unsigned char max,
                                    const size_t length) {
  if (max < min) {
    fprintf(stderr,
            "\ngenerate_test_ustring(): Invalid values for range (%u is NOT "
            "GREATER THAN %u)",
            max, min);
    return NULL;
  }
  if (length == 0) {
    fprintf(stderr, "\ngenerate_test_ustring(): Invalid length: %llu", length);
    return NULL;
  }

  /* (length + 1) to account for the null terminator. */
  unsigned char *str = malloc(length + 1);
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

// Returns a random string of signed chars using `random_vis_uchar()`.
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

// Returns a random string of alphabetical chararacters using
// random_vis_uchar().
char *randomAlphabeticalString(const size_t length) {
  if (length == 0) {
    fprintf(stderr, "\ngenerate_test_string_alphabetic(): Invalid length: %llu",
            length);
    return NULL;
  }

  // (length + 1) to account for the null terminator
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
