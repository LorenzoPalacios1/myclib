#ifndef STACK_H
#define STACK_H

#include <stdlib.h>

typedef struct {
  void *data;
  size_t capacity;
  size_t used_capacity;
  size_t elem_size;
  size_t length;
} stack;

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
stack *new_stack(size_t num_elems, size_t elem_size);

/*
 * Frees the memory used by `stk` and invalidates the passed pointer
 * associated with it.
 */
void delete_stack(stack **const stk);

/*
 * Same as `delete_stack()`, except this function will write zeros to the memory
 * used by `stk` before freeing.
 */
void delete_stack_s(stack **stk);

/*
 * Expands the memory used by `stk->data`, thereby increasing its capacity.
 *
 * \return A pointer associated with the contents of `stk` or `NULL` upon
 * failure.
 */
stack *expand_stack(stack *stk);

/*
 * Resizes the memory used by `stk->data` to `new_size`.
 *
 * \return A pointer associated with the contents of `stk` or `NULL` upon
 * failure.
 * \note If `new_size` is not a multiple of `stk->elem_size`, the closest
 * multiple to `elem_size` (rounded up) is used instead.
 */
stack *resize_stack(stack *stk, size_t new_size);

/*
 * Shrinks the memory used by `stk->data` to `stk->used_capacity`.
 *
 * \return A pointer associated with the contents of `stk` or `NULL` upon
 * failure.
 */
stack *shrink_stack_to_fit(stack *stk);

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
 * \return A pointer associated with the contents of `stk` or `NULL` upon
 * failure.
 */
stack *stack_push(stack *stk, const void *const elem);

/* Define this macro to include support for stacks of automatic storage. */
#ifdef STACK_WANT_LOCAL_STACK
/*
 * Creates a stack with automatic storage duration at `local_stk`, thereby
 * avoiding allocation on the heap.
 *
 * \param local_stk A pointer to a local variable of type `stack`.
 * \param num_elems The maximum number of elements the stack will contain.
 * \param _elem_size The size of each element in the stack.
 */
#define new_stack_no_heap(local_stk, num_elems, _elem_size)

/*
 * Returns the top element of `stk` without removing it.
 *
 * \return A pointer to the top element in `stk` or `NULL` if the end of the
 * stack was reached.
 */
void *no_heap_stack_peek(stack *stk);

/*
 * Returns and removes the top element from `stk`.
 *
 * \return A pointer to the top element in `stk` or `NULL` if the end of the
 * stack was reached.
 */
void *no_heap_stack_pop(stack *stk);

/*  
 * Adds a new element to `stk` if space permits.
 *
 * \return `stk` if the element was added successfully or `NULL` upon failure.
 */
stack *no_heap_stack_push(stack *stk, const void *const elem);

#endif
#endif
