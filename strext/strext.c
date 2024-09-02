#include "strext.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

string_t *append_char(string_t *dst, const char appended) {
  if (dst->length == dst->capacity) {
    string_t *reallocated_mem = expand_string(dst);
    if (reallocated_mem == NULL) return NULL;
    dst = reallocated_mem;
  }
  dst->data[dst->length] = appended;
  dst->length++;
  dst->data[dst->length] = '\0';
  return dst;
}

string_t *append_str(string_t *dst, const string_t *const src) {
  const size_t SRC_LEN = src->length;
  size_t DST_CAPACITY_TEMP = dst->capacity;

  while (DST_CAPACITY_TEMP - dst->length < SRC_LEN)
    DST_CAPACITY_TEMP *= dst->expansion_factor;
  if (DST_CAPACITY_TEMP != dst->capacity)
    dst = resize_string(dst, DST_CAPACITY_TEMP);
  if (dst == NULL) return NULL;
  strcat(dst->data + dst->length, src->data);
  dst->length += SRC_LEN;
  return dst;
}

string_t *append_raw_str(string_t *dst, const char *src, const size_t src_len) {
  const size_t SRC_LEN = src_len;
  size_t DST_CAPACITY_TEMP = dst->capacity;

  while (DST_CAPACITY_TEMP - dst->length < SRC_LEN)
    DST_CAPACITY_TEMP *= dst->expansion_factor;
  if (DST_CAPACITY_TEMP != dst->capacity)
    dst = resize_string(dst, DST_CAPACITY_TEMP);
  if (dst == NULL) return NULL;
  strcat(dst->data + dst->length, src);
  dst->length += SRC_LEN;
  return dst;
}

void _delete_string(string_t **str_obj) {
  free(*str_obj);
  *str_obj = NULL;
}

void _delete_string_s(string_t **str_obj) {
  memset(*str_obj, 0, (*str_obj)->capacity + sizeof(string_t));
  _delete_string(str_obj);
}

string_t *erase_string_contents(string_t *const str) {
  str->data -= str->length;
  str->length = 0;
  return str;
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

/*
 * A fix is needed for when `new_size` is equal to zero. This often causes
 * segmentation faults with any functions reliant upon a null terminator due to
 * no such character being present (because there isn't any space allocated for
 * one).
 */
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
  return string_from_stream_given_delim(stdin, '\n');
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

string_t *string_from_stream_given_delim(FILE *const stream, const char delim) {
  string_t *const str_obj = new_string(BASE_STR_CAPACITY + sizeof(string_t));
  char *const str_actual = str_obj->data;

  size_t i = 0;

  while (true) {
    const int c = getc(stream);
    if (c == delim || c == EOF) break;
    append_char(str_obj, c);
    i++;
  }
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
