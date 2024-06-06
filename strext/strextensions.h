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
#define newString(arg)                              \
  _Generic((arg),                                   \
  char *: string_from_chars(arg),                   \
  FILE *: string_from_input_stream(arg),            \
  int: string_preallocated(arg),                  \
  size_t: string_preallocated(arg),                 \
  )(arg)
/* clang-format on */

string_t *findReplace(string_t *const haystack, const string_t *const needle,
                      const string_t *const replacer);

/* To be added. */
string_t *findReplaceAll(string_t *const haystack, const string_t *const needle,
                         const string_t *const replacement);

string_t *string_from_chars(const char *const raw_text);

string_t *string_preallocated(const size_t size_in_bytes);

string_t *string_from_input_stream(FILE *const stream);
