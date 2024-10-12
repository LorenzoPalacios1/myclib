#ifndef STACKS_H
#define STACKS_H

#include <stddef.h>
#include <stdlib.h>

typedef struct {
  void *data;
  size_t capacity;
  size_t elem_size;
  size_t pos;
} stack;

#include <stddef.h>

/*
 * This is a convenience macro for `_stack_from_arr()`.
 * Use with caution if `arr` has side effects.
 */
#define stack_from_arr(arr) \
  _stack_from_arr(arr, sizeof(arr) / sizeof *(arr), sizeof *(arr))

/* Creates a stack based off the elements in `arr`. */
stack *_stack_from_arr(const void *arr, size_t len, size_t elem_size);

/*
 * Creates a new `stack` with enough capacity for `num_elems` elements
 * where each element is of size `elem_size`. The stack will be empty.
 *
 * \return A pointer to an empty `stack` or `NULL` upon failure.
 */
stack *create_stack(size_t num_elems, size_t elem_size);

/*
 * Returns the top element of `stk` without removing it.
 *
 * \return A pointer to the top element in `stk` or `NULL` if the end of the
 * stack was reached.
 */
void *stack_peek(stack *stk);

/*
 * Returns and removes the top element from `stk`.
 *
 * \return A pointer to the top element in `stk` or `NULL` if the end of the
 * stack was reached.
 */
void *stack_pop(stack *stk);

/* 
 * Adds a new element to `stk`, expanding if necessary. 
 *
 * \return A pointer associated with the contents of `stk` or `NULL` upon failure.
 */
stack *stack_push(stack *stk);

void delete_stack(stack *stk);

void delete_stack_s(stack *stk);

stack *expand_stack(stack *stk);

stack *resize_stack(stack *stk);

stack *shrink_stack(stack *stk);

#endif
