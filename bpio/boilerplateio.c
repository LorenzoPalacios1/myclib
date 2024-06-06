#include "boilerplateio.h"

#include <limits.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../strext/strextensions.h"

#include "../MyBasics.h"

size_t getStr(const char delim, const size_t max_length,
              FILE *const stream) {
  if (stream == NULL) {
    fputs("\ngetStr(): Invalid input stream; No reading occurred", stderr);
    return 0;
  }
  if (max_length == 0) return 0;
  if (feof(stream)) return 0;

  string_t *const new_str = newString("");

  size_t i = 0;
  for (; i < max_length; ++i) {
    (new_str->data)[i] = getc(stream);
    if (str[i] == delim || str[i] == EOF) break;
  }
  str[i] = '\0';

  return i;
}

string_t *getStrStdin(const size_t max_length) {
  string_t *const new_str = getStr('\n', max_length, stdin);
  if (new_str->length > max_length)
    while (getchar() != '\n');
  return new_str;
}
