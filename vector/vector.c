#include "vector.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

vector_t *add_elem(vector_t *dest, void *elem) {
  if (dest->length == dest->capacity) {
    dest = expand_vector(dest);
    if (dest == NULL) return NULL;
  }
  memcpy((char *)dest->data + dest->length * dest->elem_size, elem,
         dest->elem_size);
  dest->length++;
  return dest;
}

inline void _delete_vector(vector_t **const vec) {
  free(*vec);
  *vec = NULL;
}

inline void delete_vector_s(vector_t *vec) {
  memset(vec, 0, vec->capacity);
  delete_vector(vec);
}

vector_t *resize_vector(vector_t *const vec, const size_t new_size) {
  vector_t *new_vec = realloc(vec, new_size + sizeof(vector_t));
  if (new_vec == NULL) return NULL;
  new_vec->capacity = new_size;
  new_vec->length = (new_size < new_vec->length) ? new_size : new_vec->length;
  new_vec->data = (char *)new_vec + sizeof(vector_t);
  return new_vec;
}

vector_t *_new_vector(const void *const data, const size_t elem_size,
                      const size_t length) {
  const size_t CAPACITY = length * elem_size;
  vector_t *vec = malloc(CAPACITY + sizeof(vector_t));
  if (vec == NULL) return NULL;
  vec->data = (char *)vec + sizeof(vector_t);
  vec->elem_size = elem_size;
  vec->length = length;
  vec->capacity = CAPACITY;
  memcpy(vec->data, data, CAPACITY);
  return vec;
}
