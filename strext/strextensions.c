#include "strextensions.h"

#include <string.h>

typedef struct string_t {
  char *data;
  size_t length;
  size_t allocated_size;
  double reallocation_multiplier;
} string_t;

string_t *findReplace(string_t *const haystack, const string_t *const needle,
                      const string_t *const replacer) {
  const ptrdiff_t needleIndex =
      strstr(haystack->data, needle->data) - haystack->data;
  if (needleIndex >= 0) {
    const size_t BYTES_REQUIRED = haystack->length + replacer->length + 1;
    char *buffer;
    if (BYTES_REQUIRED > haystack->allocated_size) {
      buffer = malloc(BYTES_REQUIRED);
      haystack->allocated_size =
          BYTES_REQUIRED * haystack->reallocation_multiplier;
    } else {
      buffer = haystack->data;
    }
    /* Copy chars up to the point of insertion */
    strncpy(buffer, haystack->data, needleIndex);
    /* Insert the replacing string */
    strcpy(buffer + needleIndex, replacer->data);
    /* Copy chars after the point of insertion */
    strcpy(buffer + needleIndex + replacer->length,
           haystack->data + needleIndex + needle->length);
    buffer[haystack->length + replacer->length] = '\0';
    haystack->length += replacer->length - needle->length;
    haystack->data = buffer;
    return haystack;
  }
  return haystack;
}
