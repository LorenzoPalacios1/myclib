#ifndef VECTOR_H
#define VECTOR_H

#include <stddef.h>

#define REALLOC_FACTOR (2)

/* clang-format off */
#define new_vector(data, length) _new_vector(data, sizeof*(data), length)
#define new_vector_from_c_arr(arr) _new_vector(arr, sizeof*(arr), sizeof(arr) / sizeof*(arr))
#define delete_vector(vec) _delete_vector(&(vec))
#define expand_vector(vec) resize_vector(vec, (vec)->capacity * REALLOC_FACTOR)
/* clang-format on */

typedef struct vector_t {
  void *data;
  size_t length;
  size_t elem_size;
  size_t capacity;
} vector_t;

void _delete_vector(vector_t **v);

void delete_vector_s(vector_t *v);

vector_t *resize_vector(vector_t *vec, size_t new_size);

size_t print_vector(const vector_t *vec);

vector_t *_new_vector(const void *data, size_t elem_size, size_t num_elems);
#endif
