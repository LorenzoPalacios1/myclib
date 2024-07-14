#include "strextensions.h"

#include <assert.h>
#include <stdio.h>
#include <string.h>

#define BASE_STR_ALLOC_SIZE (20) /* In bytes. */
#define BASE_REALLOC_FACTOR (2)

void append_char_to_string(string_t *str_obj, const char appended) {
  char *raw_str = str_obj->data;
  if (str_obj->length == str_obj->str_alloc_size) {
    string_t *reallocated_mem = resize_string(
        str_obj, str_obj->realloc_factor * str_obj->str_alloc_size);
    if (reallocated_mem == NULL) return;
    str_obj = reallocated_mem;
  }
  raw_str[str_obj->length] = appended;
  str_obj->length++;
  raw_str[str_obj->length] = '\0';
}

/* Expands the passed string's allocated boundaries by `realloc_factor`. */
string_t *expand_string(string_t *str) {
  return resize_string(str, str->realloc_factor * str->str_alloc_size);
}

string_t *find_replace(string_t *const haystack, const string_t *const needle,
                       const string_t *const replacement) {
  /* These are shorthands for accessing often used variables. */
  const char *const replacer = replacement->data;
  const char *const to_be_replaced = needle->data;
  const size_t REPLACER_LEN = replacement->length;
  const size_t NEEDLE_LEN = needle->length;
  char *hay = haystack->data;
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
      if (haystack->str_alloc_size < BYTES_REQUIRED) {
        haystack->str_alloc_size = BYTES_REQUIRED * haystack->realloc_factor;
        hay = realloc(hay, haystack->str_alloc_size);
        if (!hay) return haystack;
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

/* 
 * This function only modifies the memory allocated for characters, not the
 * rest of the struct.
 * The actual size of any string can be no lower than sizeof(string_t).
 */
string_t *resize_string(string_t *str, const size_t new_size) {
  string_t *new_mem = realloc(str, new_size + sizeof(string_t));
  if (new_mem == NULL) return NULL;
  new_mem->str_alloc_size = new_size;
  new_mem->data = (char *)new_mem + sizeof(string_t);
  return new_mem;
}

string_t *shrink_alloc_to_length(string_t *str) {
  return resize_string(str, str->length);
}

string_t *string_from_chars(const char *const raw_text) {
  string_t *str_obj = malloc(BASE_STR_ALLOC_SIZE + sizeof(string_t));
  if (str_obj == NULL) return NULL;
  str_obj->str_alloc_size = BASE_STR_ALLOC_SIZE;
  str_obj->realloc_factor = BASE_REALLOC_FACTOR;
  str_obj->data = (char *)str_obj + sizeof(string_t);

  size_t i = 0;
  for (; raw_text[i] != '\0'; i++) {
    if (i == str_obj->str_alloc_size) {
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
  string_t *str_obj = new_string(BASE_STR_ALLOC_SIZE);
  char *str_actual = str_obj->data;

  char c = getc(stream);
  for (size_t i = 0; c != EOF; i++) {
    if (i == str_obj->str_alloc_size) {
      string_t *reallocated_mem = resize_string(
          str_obj, str_obj->realloc_factor * str_obj->str_alloc_size);
      if (reallocated_mem == NULL) return NULL;
      str_obj = reallocated_mem;
    }
    str_actual[i] = c;
    c = getc(stream);
  }
  return str_obj;
}

string_t *string_from_stream_given_delim(const char delim, FILE *const stream) {
  string_t *const str_obj = new_string(BASE_STR_ALLOC_SIZE);
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

string_t *string_preallocated(const size_t size_in_bytes) {
  string_t *str_obj = malloc(size_in_bytes + sizeof(string_t));
  if (str_obj == NULL) return NULL;
  str_obj->data = (char *)str_obj + sizeof(string_t);
  str_obj->length = 0;
  str_obj->str_alloc_size = size_in_bytes;
  str_obj->realloc_factor = BASE_REALLOC_FACTOR;
  return str_obj;
}
