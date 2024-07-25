#include "strextensions.h"

#include <assert.h>
#include <stdio.h>
#include <string.h>

string_t *append_char_to_string(string_t *str_obj, const char appended) {
  char *raw_str = str_obj->data;
  if (str_obj->length == str_obj->capacity) {
    string_t *reallocated_mem = expand_string(str_obj);
    if (reallocated_mem == NULL) return NULL;
    str_obj = reallocated_mem;
  }
  raw_str[str_obj->length] = appended;
  str_obj->length++;
  raw_str[str_obj->length] = '\0';
  return str_obj;
}

string_t *expand_string(string_t *str_obj) {
  return resize_string(str_obj, str_obj->expansion_factor * str_obj->capacity);
}

string_t *find_replace(string_t *haystack, const string_t *const needle,
                       const string_t *const replacement) {
  const char *const replacer = replacement->data;
  const char *const to_be_replaced = needle->data;
  const size_t REPLACER_LEN = replacement->length;
  const size_t NEEDLE_LEN = needle->length;
  char *hay = haystack->data;
  /* The value of `HAY_LEN` is subject to change during string replacement. */
  const size_t HAY_LEN = haystack->length;

  /*
   * There exists a bug where if `needle` is a zero-length string (that is, a
   * string containing only a null terminator), `replacement` will be prepended
   * to `haystack`.
   * Beyond that, this statement only serves to terminate the function if any
   * passed strings are of length 0, since no meaningful operation can be had.
   */
  if (HAY_LEN == 0 || NEEDLE_LEN == 0 || REPLACER_LEN == 0) return haystack;

  const ptrdiff_t needle_index = strstr(hay, to_be_replaced) - hay;
  if (needle_index >= 0) {
    {
      const size_t BYTES_REQUIRED = REPLACER_LEN - NEEDLE_LEN;
      if (haystack->capacity < BYTES_REQUIRED) {
        string_t *reallocated_mem = expand_string(haystack);
        if (reallocated_mem == NULL) return NULL;
        haystack = reallocated_mem;
      }
    }
    char suffixed_chars[HAY_LEN - needle_index - NEEDLE_LEN];
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

string_t *resize_string(string_t *str_obj, const size_t new_size) {
  string_t *new_mem = realloc(str_obj, new_size + sizeof(string_t));
  if (new_mem == NULL) return NULL;
  new_mem->capacity = new_size;
  new_mem->data = (char *)new_mem + sizeof(string_t);
  return new_mem;
}

string_t *shrink_alloc_to_length(string_t *str_obj) {
  return resize_string(str_obj, str_obj->length);
}

string_t *string_from_chars(const char *const raw_text) {
  string_t *str_obj = malloc(BASE_STR_CAPACITY + sizeof(string_t));
  if (str_obj == NULL) return NULL;
  str_obj->capacity = BASE_STR_CAPACITY;
  str_obj->expansion_factor = BASE_EXPANSION_FACTOR;
  str_obj->data = (char *)str_obj + sizeof(string_t);

  /*
   * `strncpy()` could simplify this loop, but it may introduce overhead as,
   * after a null terminator is reached, it will "fill in" any unused space
   * within `str_obj` with null characters. Currently, this loop writes only
   * what is necessary.
   */
  size_t i = 0;
  for (; raw_text[i] != '\0'; i++) {
    if (i == str_obj->capacity) {
      string_t *new_mem = expand_string(str_obj);
      if (new_mem == NULL) return NULL;
      str_obj = new_mem;
    }
    str_obj->data[i] = raw_text[i];
  }
  str_obj->data[i] = '\0';
  str_obj->length = i;
  return str_obj;
}

string_t *string_from_line_stdin(void) {
  return string_from_stream_given_delim('\n', stdin);
}

string_t *string_from_stream(FILE *const stream) {
  string_t *str_obj = new_string(BASE_STR_CAPACITY + sizeof(string_t));
  char *str_actual = str_obj->data;

  char c = getc(stream);
  size_t i = 0;
  for (; c != EOF; i++) {
    if (i == str_obj->capacity) {
      string_t *reallocated_mem =
          resize_string(str_obj, str_obj->expansion_factor * str_obj->capacity);
      if (reallocated_mem == NULL) return NULL;
      str_obj = reallocated_mem;
    }
    str_actual[i] = c;
    c = getc(stream);
  }
  str_obj->length = i;
  return str_obj;
}

string_t *string_from_stream_given_delim(const char delim, FILE *const stream) {
  string_t *const str_obj = new_string(BASE_STR_CAPACITY + sizeof(string_t));
  char *const str_actual = str_obj->data;

  size_t i = 0;
  do {
    const int c = getc(stream);
    if (c == delim || c == EOF) break;
    append_char_to_string(str_obj, c);
    i++;
  } while (true);
  str_actual[i] = '\0';
  str_obj->length = i;

  return str_obj;
}

string_t *string_of_capacity(const size_t capacity) {
  string_t *str_obj = malloc(capacity + sizeof(string_t));
  if (str_obj == NULL) return NULL;
  str_obj->data = (char *)str_obj + sizeof(string_t);
  str_obj->length = 0;
  str_obj->capacity = capacity;
  str_obj->expansion_factor = BASE_EXPANSION_FACTOR;
  return str_obj;
}
