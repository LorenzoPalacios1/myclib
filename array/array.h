#ifndef ARRAY_H
#define ARRAY_H

#include <stddef.h>

typedef struct array_t {
  void *data;
  size_t allocated_size;
  size_t elem_size;
  size_t num_elems;
} array_t;

array_t *new_array(const void *data, size_t elem_size,
                   size_t num_elems);

void *get_elem(const array_t *arr, size_t index);

void delete_array(array_t **arr);

void secure_delete_array(array_t **arr);

void clear_array_contents(array_t *arr);

#endif
