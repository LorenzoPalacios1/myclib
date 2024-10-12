#ifndef BACKWARDSTACK_H
#define BACKWARDSTACK_H

#include <stddef.h>

#include "../stack.h"

/*
 * This is a convenience macro for `_bwd_stack_from_arr()`.
 * Use with caution if `arr` has side effects.
 */
#define bwd_stack_from_arr(arr) \
  _bwd_stack_from_arr(arr, sizeof(arr) / sizeof *(arr), sizeof *(arr))

/*
 * Defines a stack whose elements are ordered in reverse of its source array.
 * That is, for an array `{1, 2, 3}`, the `bwd_stack` would be
 *
 * `{NULL, 3, 2, 1}`
 *
 * where `1` denotes the start of the stack and `NULL` denotes the end of the
 * stack. This is in contrast to `fwd_stack` whose stack layout would be
 *
 * `{1, 2, 3, NULL}`
 *
 * where `1` denotes the start of the stack and `NULL` denotes the end of the
 * stack.
 */
typedef stack bwd_stack;

/*
 * Creates a new `bwd_stack` with enough capacity for `num_elems` elements of
 * size `elem_size`. The stack will be empty.
 *
 * \return A pointer to an empty `bwd_stack`
 */
bwd_stack *create_bwd_stack(size_t num_elems, size_t elem_size);

/*
 * Returns the top element of `stk` without removal.
 *
 * \return A pointer to the top element in `stk` or `NULL` if the end of the
 * stack was reached.
 */
void *bwd_stack_peek(bwd_stack *stk);

/*
 * Returns and removes the top element from `stk`.
 *
 * \return A pointer to the top element in `stk` or `NULL` if the end of the
 * stack was reached.
 */
void *bwd_stack_pop(bwd_stack *stk);

void bwd_stack_push(bwd_stack *stk);

bwd_stack *_bwd_stack_from_arr(const void *data, size_t len, size_t elem_size);

bwd_stack *create_bwd_stack(size_t num_elems, size_t elem_size);

void delete_bwd_stack(bwd_stack *stk);

void delete_bwd_stack_s(bwd_stack *stk);

bwd_stack *expand_bwd_stack(bwd_stack *stk);

bwd_stack *resize_bwd_stack(bwd_stack *stk);

bwd_stack *shrink_bwd_stack(bwd_stack *stk);

#endif
