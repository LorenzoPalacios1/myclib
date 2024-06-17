#include "array.h"

array_t *new_array(const void *const data, const size_t elem_size,
                   const size_t num_elems) {
  array_t *new_arr = malloc(sizeof(array_t));
  new_arr->num_elems = num_elems;
  new_arr->allocated_size = (elem_size * num_elems);
  new_arr->elem_size = elem_size;
  new_arr->data = malloc(new_arr->allocated_size);

  if (data != NULL) memcpy(new_arr->data, data, num_elems * elem_size);
  return new_arr;
}

void *get_elem(const array_t *const arr, const size_t index) {
  if (index >= arr->num_elems) return NULL;
  return (char *)arr->data + (arr->elem_size * index);
}

void delete_array(array_t *arr) {
  free(arr->data);
  free(arr);
  arr = NULL;
}

void secure_delete_array(array_t *arr) {
  memset(arr->data, 0, arr->allocated_size);
  free(arr->data);
  memset(arr, 0, sizeof(array_t));
  free(arr);
  arr = NULL;
}

void clear_array(array_t *const arr) {
  memset(arr->data, 0, arr->allocated_size);
  arr->num_elems = 0;
}
