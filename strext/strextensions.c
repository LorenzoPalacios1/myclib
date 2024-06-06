#include "strextensions.h"

#include <string.h>

#define BASE_ALLOCATION (2048) /* In bytes. */
#define BASE_REALLOCATION_MULITPLIER (2)

/* clang-format off */
#define newString(arg)                                \
  _Generic((arg),                                     \
  char *: new_string_from_chars(arg)                  \
  FILE *: new_string_from_input_stream(arg)           \
  int: new_string_preallocated(arg)                   \
  size_t: new_string_preallocated(arg)                \
  default: NULL)
/* clang-format on */

static string_t *new_string_from_chars(const char *const raw_text) {
  string_t *str_obj = malloc(sizeof(string_t));
  char *str_contents = malloc(BASE_ALLOCATION);
  if (!str_obj || !str_contents) return NULL;
  str_obj->allocated_bytes = BASE_ALLOCATION;
  str_obj->reallocation_multiplier = BASE_REALLOCATION_MULITPLIER;

  size_t i = 0;
  for (; raw_text[i]; i++) {
    if (i == str_obj->allocated_bytes) {
      str_obj->allocated_bytes *= str_obj->reallocation_multiplier;
      str_contents = realloc(str_contents, str_obj->allocated_bytes);
      if (!str_contents) return NULL;
    }
    str_contents[i] = raw_text[i];
  }
  str_contents[i] = '\0';
  str_obj->data = str_contents;
  str_obj->length = i;
  return str_obj;
}

static string_t *new_string_preallocated(const size_t size_in_bytes) {
  string_t *str_obj = malloc(sizeof(string_t));
  char *str_contents = malloc(size_in_bytes);
  if (!str_obj || !str_contents) return NULL;
  str_obj->allocated_bytes = size_in_bytes;
  str_obj->reallocation_multiplier = BASE_REALLOCATION_MULITPLIER;
  str_obj->data = str_contents;
  str_obj->length = 0;
  return str_obj;
}

string_t *shrinkAllocationToLength(string_t *const str) {
  if (str->length < str->allocated_bytes - 1)
    str->data = realloc(str->data, str->length);
  return str;
}

string_t *findReplace(string_t *const haystack, const string_t *const needle,
                      const string_t *const replacement) {
  /* These are shorthands for accessing often used variables. */
  const char *const replacer = replacement->data;
  const char *const to_be_replaced = needle->data;
  char *hay = haystack->data;
  const size_t REPLACER_LEN = replacement->length;
  const size_t NEEDLE_LEN = needle->length;
  /*
   * The length of the haystack can change during string insertion, hence this
   * being a pointer instead of a constant.
   */
  size_t *const HAY_LEN = &(haystack->length);

  /*
   * There exists a bug where if `needle` is a zero-length string (that is, a
   * string containing only a null terminator), `replacement` will be prepended
   * to `haystack`.
   * Beyond that, this statement only serves to terminate the function if any
   * passed strings are of length 0, since no meaningful operation can be had.
   */
  if (*HAY_LEN == 0 || NEEDLE_LEN == 0 || REPLACER_LEN == 0) return haystack;

  const ptrdiff_t needle_index = strstr(hay, to_be_replaced) - hay;
  if (needle_index >= 0) {
    {
      const size_t BYTES_REQUIRED = *HAY_LEN + REPLACER_LEN - NEEDLE_LEN;
      if (haystack->allocated_bytes < BYTES_REQUIRED) {
        haystack->allocated_bytes =
            BYTES_REQUIRED * haystack->reallocation_multiplier;
        hay = realloc(hay, haystack->allocated_bytes);
      }
    }
    char suffixed_chars[*HAY_LEN - needle_index - NEEDLE_LEN];
    /* Copy chars up to the point of insertion. */
    strcpy(suffixed_chars, haystack->data + needle_index + NEEDLE_LEN);
    /* Insert the replacement string. */
    strcpy(hay + needle_index, replacer);
    /* Replace any chars after the point of insertion. */
    strcpy(hay + needle_index + REPLACER_LEN, suffixed_chars);
    haystack->length += REPLACER_LEN - NEEDLE_LEN;
    haystack->data = hay;
  }
  return haystack;
}

static string_t *string_from_input_stream(FILE *const stream) {

}
