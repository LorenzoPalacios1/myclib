#include "strextensions.h"

#include <string.h>

/* In bytes. */
#define BASE_ALLOCATION (512)
#define BASE_REALLOCATION_MULITPLIER (2)

string_t *newString(const char *const raw_str) {
  string_t *str = malloc(sizeof(string_t));
  str->allocated_bytes = BASE_ALLOCATION;
  str->reallocation_multiplier = BASE_REALLOCATION_MULITPLIER;

  char *buffer = malloc(str->allocated_bytes);
  if (!buffer) return NULL;

  size_t i = 0;
  for (; raw_str[i]; i++) {
    if (i == str->allocated_bytes) {
      str->allocated_bytes *= str->reallocation_multiplier;
      buffer = realloc(buffer, str->allocated_bytes);
      if (!buffer) return NULL;
    }
    buffer[i] = raw_str[i];
  }
  buffer[i] = '\0';
  str->data = buffer;
  str->length = i;
  return str;
}

string_t *findReplace(string_t *const haystack, const string_t *const needle,
                      const string_t *const replacement) {
  const ptrdiff_t needleIndex =
      strstr(haystack->data, needle->data) - haystack->data;
  if (needleIndex >= 0) {
    const size_t BYTES_REQUIRED =
        haystack->length + replacement->length - needle->length;
    char *buffer = haystack->data;
    if (BYTES_REQUIRED > haystack->allocated_bytes) {
      haystack->allocated_bytes =
          BYTES_REQUIRED * haystack->reallocation_multiplier;
      buffer = realloc(haystack->data, haystack->allocated_bytes);
    }
    /* Insert the replacement string */
    strcpy(buffer + needleIndex, replacement->data);
    /* Copy chars after the point of insertion */
    if (needle->length > replacement->length)
      strcpy(buffer + needleIndex + replacement->length,
             haystack->data + needleIndex + needle->length);
    else
      strcpy(buffer + needleIndex + replacement->length,
             haystack->data + needleIndex + replacement->length);
    haystack->length += replacement->length - needle->length;
    buffer[haystack->length] = '\0';
    haystack->data = buffer;
  }
  return haystack;
}
