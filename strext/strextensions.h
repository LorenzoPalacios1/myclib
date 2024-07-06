#ifndef _STRING_BASICS
#define _STRING_BASICS

#include <stdio.h>
#include <string.h>

#include "../MyBasics.h"

typedef struct string_t {
  char *data;
  size_t length;
  size_t allocated_bytes;
  double reallocation_multiplier;
} string_t;

/* clang-format off */
#define new_string(arg)                 \
  (_Generic((arg),                      \
  char *: string_from_chars,            \
  FILE *: string_from_stream,           \
  int: string_preallocated,             \
  size_t: string_preallocated           \
  )(arg))
/* clang-format on */

void append_char_to_string(string_t *const str_obj, const char appended);

string_t *find_replace(string_t *const haystack, const string_t *const needle,
                       const string_t *const replacer);

/* To be added. */
string_t *find_replace_all(string_t *const haystack,
                           const string_t *const needle,
                           const string_t *const replacement);

string_t *string_from_chars(const char *const raw_text);

string_t *string_preallocated(const size_t size_in_bytes);

string_t *string_from_stream(FILE *const stream);

/*
 * \returns A string whose content consists of characters within `stream` until
 * `delim` is read.
 *
 * \note A null terminator will replace `delim` where it would have occurred
 * within the string.
 *
 * \param delim The delimiter character
 * \param stream The input stream to read from
 */
string_t *string_from_stream_given_delim(const char delim, FILE *const stream);

/*
 * Returns a string whose content consists of characters within `stdin` until
 * reading a newline character.
 */
string_t *string_from_line_stdin(void);

#endif
