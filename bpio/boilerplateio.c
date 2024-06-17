#include "boilerplateio.h"

#include <limits.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../MyBasics.h"
#include "../strext/strextensions.h"

/* I'm considering moving these functions to strext. */

string_t *get_str(const char delim, const size_t max_length,
                  FILE *const stream) {
  if (stream == NULL) {
    fputs("\ngetStr(): Invalid input stream; No reading occurred", stderr);
    return 0;
  }
  if (max_length == 0) return 0;
  if (feof(stream)) return 0;

  string_t *const new_str = new_string(max_length);
  char *const str_contents = new_str->data;

  int c;
  size_t i = 0;
  for (; i < max_length; ++i) {
    c = getc(stream);
    if (c == delim || c == EOF) break;
    str_contents[i] = c;
  }
  str_contents[i] = '\0';
  new_str->length = i;

  return new_str;
}

string_t *get_str_stdin(const size_t max_length) {
  string_t *const new_str = get_str('\n', max_length, stdin);
  if (new_str->length > max_length)
    while (getchar() != '\n');
  return new_str;
}
