#include "array.h"

#include <stddef.h>
#include <stdlib.h>
#include <string.h>

array_t *_new_array(const void *const data, const size_t elem_size,
                   const size_t length) {
  array_t *const new_arr = malloc(elem_size * length + sizeof(array_t));
  new_arr->capacity = elem_size * length;
  new_arr->elem_size = elem_size;
  new_arr->length = length;
  new_arr->data = (char *)new_arr + sizeof(array_t);

  if (data != NULL) memcpy(new_arr->data, data, elem_size * length);
  return new_arr;
}

void *get_elem(const array_t *const arr, const size_t index) {
  if (index >= arr->length) return NULL;
  return (char *)arr->data + (arr->elem_size * index);
}

void _delete_array(array_t **const arr) {
  free(*arr);
  *arr = NULL;
}

void delete_array_s(array_t *arr) {
  memset(arr, 0, arr->capacity + sizeof(*arr));
  delete_array(arr);
}

void clear_array_contents(array_t *const arr) {
  memset(arr->data, 0, arr->capacity);
  arr->length = 0;
}
