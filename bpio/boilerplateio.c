#include "boilerplateio.h"

#include <limits.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../MyBasics.h"

size_t getStr(char *str, const char delim, const size_t max_length,
              FILE *const stream) {
  if (stream == NULL) {
    fputs("\ngetStr(): Invalid input stream; No reading occurred", stderr);
    return 0;
  }
  if (max_length == 0) return 0;
  if (feof(stream)) return 0;

  if (str == NULL) {
    /* `max_length + 1` to ensure space for a null terminator */
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
  if (str_len > max_length)
    while (getchar() != '\n');
  return str_len;
}
