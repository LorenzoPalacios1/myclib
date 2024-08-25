#ifndef ARRAY_H
#define ARRAY_H

#include <stddef.h>

#define new_array(data, length) _new_array(data, sizeof*(data), length)
#define delete_array(arr) _delete_array(&(arr))

typedef struct array_t {
  void *data;
  size_t capacity;
  size_t length;
  size_t elem_size;
} array_t;

array_t *_new_array(const void *data, size_t elem_size, size_t length);

void *get_elem(const array_t *arr, size_t index);

void _delete_array(array_t **arr);

void delete_array_s(array_t *arr);

void clear_array_contents(array_t *arr);

#endif
