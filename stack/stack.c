#include "stack.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef unsigned char byte_t;

static stack *alloc_stack(const size_t stack_capacity) {
  return malloc(stack_capacity + sizeof(stack));
}

stack *create_stack(const size_t num_elems, const size_t elem_size) {
  const size_t STACK_CAPACITY = num_elems * elem_size;
  stack *const stk = alloc_stack(STACK_CAPACITY);
  if (stk == NULL) return NULL;
  stk->capacity = STACK_CAPACITY;
  stk->used_capacity = 0;
  stk->elem_size = elem_size;
  stk->data = stk + 1;
  stk->length = 0;
  return stk;
}

stack *_stack_from_arr(const void *const arr, const size_t len,
                       const size_t elem_size) {
  stack *const stk = create_stack(len, elem_size);
  if (stk == NULL) return NULL;
  stk->length = len;
  stk->used_capacity = elem_size * len;
  /*
   * Since a backward stack requires its constituent elements to be
   * laid out in reverse order, we iterate from the last element of
   * `arr` to its first.
   */
  for (size_t stk_i = 0, arr_i = len - 1; stk_i < len; stk_i++, arr_i--) {
    memcpy((byte_t *)stk->data + stk_i * elem_size,
           (byte_t *)arr + arr_i * elem_size, elem_size);
  }
  return stk;
}

void *stack_peek(stack *const stk) {
  if (stk->length == 0) return NULL;
  /*
   * Subtracting by one since `length` is equivalent to the number of elements
   * within `stk` and we need a length of zero to denote an empty stack.
   */
  return (byte_t *)stk->data + (stk->length - 1) * stk->elem_size;
}

void *stack_pop(stack *const stk) {
  if (stk->length == 0) return NULL;
  void *val = stack_peek(stk);
  stk->length--;
  return val;
}

stack *stack_push(stack *stk, const void *const elem) {
  if (stk->used_capacity + stk->elem_size >= stk->capacity) {
    stk = expand_stack(stk);
    if (stk == NULL) return NULL;
  }
  const size_t LENGTH = stk->length;
  const size_t ELEM_SIZE = stk->elem_size;
  memcpy((byte_t *)stk->data + LENGTH * ELEM_SIZE, elem, ELEM_SIZE);
  stk->length++;
  stk->used_capacity += stk->elem_size;
  return stk;
}

void delete_stack(stack **const stk) {
  free(*stk);
  *stk = NULL;
}

void delete_stack_s(stack **stk) {
  memset(*stk, 0, (*stk)->capacity);
  delete_stack(stk);
}

int main(void) {
  const size_t data[] = {1,  2,  3,  4,  5,  6,  7,  8,  9,  10, 11, 12, 13, 14,
                         15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28,
                         29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40};
  stack *a = stack_from_arr(data);
  const size_t num = 3;
  a = stack_push(a, &num);
  const size_t *val;
  while ((val = stack_pop(a))) printf("%zu\n", *val);
  return 0;
}
