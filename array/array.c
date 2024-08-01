#include "array.h"

#include <stdlib.h>
#include <string.h>
#include <stddef.h>

array_t *new_array(const void *const data, const size_t elem_size,
                   const size_t num_elems) {
  array_t *const new_arr = malloc(elem_size * num_elems + sizeof(array_t));

  new_arr->allocated_size = elem_size * num_elems;
  new_arr->elem_size = elem_size;
  new_arr->num_elems = num_elems;
  new_arr->data = (char *)new_arr + sizeof(array_t);

  if (data != NULL) memcpy(new_arr->data, data, elem_size * num_elems);
  return new_arr;
}

void *get_elem(const array_t *const arr, const size_t index) {
  if (index >= arr->num_elems) return NULL;
  return (char *)arr->data + (arr->elem_size * index);
}

void delete_array(array_t *arr) {
  free(arr);
  arr = NULL;
}

void secure_delete_array(array_t *arr) {
  memset(arr, 0, arr->allocated_size + sizeof(*arr));
  free(arr);
  arr = NULL;
}

void clear_array_contents(array_t *const arr) {
  memset(arr->data, 0, arr->allocated_size);
}
