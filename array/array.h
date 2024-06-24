#ifndef ARRAY_H
#define ARRAY_H

#include <string.h>

#include "../MyBasics.h"

typedef struct array_t {
  void *data;
  size_t allocated_size;
  size_t elem_size;
  size_t num_elems;
} array_t;

array_t *new_array(const void *const data, const size_t elem_size,
                   const size_t num_elems);

void *get_elem(const array_t *const arr, const size_t index);

void delete_array(array_t *arr);

void secure_delete_array(array_t *arr);

void clear_array(array_t *const arr);

#endif
