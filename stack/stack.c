#define STACK_WANT_LOCAL_STACK
#include "stack.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* The factor by which to scale a stack's capacity by when expanding. */
#define STACK_EXPANSION_FACTOR (2)

typedef unsigned char byte_t;

static stack *alloc_stack(const size_t stack_capacity) {
  return malloc(stack_capacity + sizeof(stack));
}

stack *new_stack(const size_t num_elems, const size_t elem_size) {
  const size_t STACK_CAPACITY = num_elems * elem_size;
  stack *const stk = alloc_stack(STACK_CAPACITY);
  if (stk == NULL) return NULL;
  stk->capacity = STACK_CAPACITY;
  stk->used_capacity = 0;
  stk->elem_size = elem_size;
  stk->data = stk + 1; /* Increment past the stack header. */
  stk->length = 0;
  return stk;
}

stack *_stack_from_arr(const void *const arr, const size_t len,
                       const size_t elem_size) {
  stack *const stk = new_stack(len, elem_size);
  if (stk == NULL) return NULL;
  stk->length = len;
  stk->used_capacity = stk->capacity;
  /*
   * Since a backward stack requires its constituent elements to be
   * laid out in reverse order, we iterate from the last element of
   * `arr` to the first.
   */
  for (size_t stk_i = 0, arr_i = len - 1; stk_i < len; stk_i++, arr_i--) {
    memcpy((byte_t *)stk->data + stk_i * elem_size,
           (byte_t *)arr + arr_i * elem_size, elem_size);
  }
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

stack *expand_stack(stack *stk) {
  stack *new_stk = resize_stack(stk, STACK_EXPANSION_FACTOR * stk->capacity);
  /*
   * Reallocation may fail because the requested memory is too large.
   * In this case, we fall back to the safer, yet generally less efficient
   * option of only allocating enough memory for one new element in the stack.
   * This has the likely side effect of requiring more later reallocations, but
   * is more likely to ensure program stability.
   * Of course, if this also fails, then chances are the system is out of
   * memory, so it's fine to return `NULL`.
   */
  if (new_stk == NULL)
    new_stk = resize_stack(stk, stk->capacity + stk->elem_size);
  return new_stk;
}

stack *resize_stack(stack *stk, size_t new_size) {
  {
    const size_t ADDITIONAL_BYTES =
        stk->elem_size - (new_size % stk->elem_size);
    if (ADDITIONAL_BYTES != 0) new_size += ADDITIONAL_BYTES;
  }
  stk = realloc(stk, new_size + sizeof(stack));
  if (stk == NULL) return NULL;
  stk->capacity = new_size;
  stk->data = stk + 1; /* Increment past the stack header. */
  return stk;
}

stack *shrink_stack_to_fit(stack *stk) {
  if (stk->capacity > stk->used_capacity)
    stk = resize_stack(stk, stk->used_capacity);
  return stk;
}

void *stack_peek(stack *const stk) {
  if (stk->length == 0) return NULL;
  /*
   * Subtracting by one since `stk->length` is equivalent to the number of
   * elements within `stk`.
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
  if (stk->used_capacity + stk->elem_size > stk->capacity) {
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

#ifdef STACK_WANT_LOCAL_STACK
#ifdef new_stack_no_heap
#undef new_stack_no_heap
#endif

/* Ensures that each stack's allocation gets a fairly unique name. */
#define STACK_NAME(local_stk) _stk_data_ ## local_stk

#define new_stack_no_heap(local_stk, num_elems, _elem_size) \
  byte_t STACK_NAME(local_stk)[(_elem_size) * (num_elems)]; \
  (local_stk) = (void *)STACK_NAME(local_stk);              \
  (*(local_stk)).data = local_stk + 1;                      \
  (*(local_stk)).capacity = sizeof(STACK_NAME(local_stk));  \
  (*(local_stk)).used_capacity = 0;                         \
  (*(local_stk)).elem_size = _elem_size;                    \
  (*(local_stk)).length = num_elems

void *no_heap_stack_peek(stack *stk);

void *no_heap_stack_pop(stack *stk);

void no_heap_stack_push(stack *stk, const void *const elem);

#endif

int main(void) {
  stack *local_stk;
  new_stack_no_heap(local_stk, 3, 8);
  stack *other_stk;
  new_stack_no_heap(other_stk, 4, 5);
  return 0;
}
