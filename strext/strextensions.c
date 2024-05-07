#include "strextensions.h"

#include <string.h>

#define BUFFER_RESIZE_FACTOR (5)

char *findAndReplace(char *const haystack, const size_t HAYSTACK_LEN,
                     const char *const needle, const size_t NEEDLE_LEN,
                     const char *const replacer, const size_t REPLACER_LEN) {
  const ptrdiff_t needleIndex = strstr(haystack, needle) - haystack;
  if (needleIndex >= 0) {
    char *buffer = malloc(HAYSTACK_LEN + REPLACER_LEN + 1);
    /* Copy chars up to the point of insertion */
    strncpy(buffer, haystack, needleIndex); 
    /* Insert the replacing string */
    strcpy(buffer + needleIndex, replacer); 
    /* Copy chars after the point of insertion */
    strcpy(buffer + needleIndex + REPLACER_LEN, haystack + needleIndex + NEEDLE_LEN);
    buffer[HAYSTACK_LEN + REPLACER_LEN] = '\0';
    return buffer;
  }
  return haystack;
}
