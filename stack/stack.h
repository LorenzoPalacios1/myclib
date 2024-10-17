#ifndef STACK_H
#define STACK_H

/*
 * Define `STACK_INCL_NO_HEAP_STACK` to include support for stacks of automatic
 * storage.
 */

#include <stdlib.h>

typedef struct {
  void *data;
  size_t capacity; /* `capacity + sizeof(stack)` will give total allocation */
  size_t used_capacity;
  size_t elem_size;
  size_t length;
} stack;

typedef unsigned char byte_t;

/*
 * This is a convenience macro for `_stack_from_arr()`.
 * Use with caution if `arr` has side effects.
 */
#define stack_from_arr(arr) \
  _stack_from_arr(arr, sizeof(arr) / sizeof *(arr), sizeof *(arr))

/* Creates a stack based off the elements in `arr`. */
stack *_stack_from_arr(const void *arr, size_t len, size_t elem_size);

/* Resets `stk->length` and `stk->used_capacity` to `0`. */
void clear_stack(stack *stk);

/*
 * Same as `clear_stack()`, except this function will overwrite the contents of
 * `stk->data` with zeros.
 */
void clear_stack_s(stack *stk);

/*
 * Frees the memory used by `stk` and invalidates the passed pointer
 * associated with it.
 */
void delete_stack(stack **const stk);

/*
 * Same as `delete_stack()`, except this function will overwrite the contents of
 * `stk` with zeros before freeing.
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
 * Creates a new stack whose `stack->data` is `data`; this function does not
 * copy the elements of `data` to an allocated block of memory. This function
 * allocates memory solely for a stack header (that is, the `stack` struct
 * type).
 *
 * \return A pointer to a stack header whose contents are associated with the
 * values at `data` or `NULL` upon failure.
 * \note Any changes made to the stack (particularly `stack_interface_push()`)
 * are reflected upon the contents of `data`.
 */
stack *interface_stack(const void *data, size_t len, size_t elem_size);

/*
 * Returns and removes the top element from `stk`.
 *
 * \return A pointer to the top element in `stk` or `NULL` if the end of the
 * stack was reached.
 */
void *interface_stack_pop(stack *stk);

/*
 * Returns, but does not remove, the top element of `stk`.
 *
 * \return A pointer to the top element in `stk` or `NULL` if the end of the
 * stack was reached.
 */
void *interface_stack_peek(stack *stk);

/*
 * Adds `elem` to `stk` if space permits (this can only be true if
 * `interface_stack_pop(stk)` was called). `elem` will then be the new top
 * element and will be returned by functions such as `interface_stack_peek()`.
 */
stack *interface_stack_push(stack *stk, const void *elem);

/*
 * Creates a new `stack` with enough capacity for `num_elems` elements
 * where each element is of size `elem_size`. The stack will be empty.
 *
 * \return A pointer to an empty `stack` or `NULL` upon failure.
 */
stack *new_stack(size_t num_elems, size_t elem_size);

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
 * Returns, but does not remove, the top element of `stk`.
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
 * Adds `elem` to `stk`, expanding if necessary. `elem` will then be the new top
 * element and will be returned by functions such as `stack_peek()`.
 *
 * \return A pointer associated with the contents of `stk` or `NULL` upon
 * failure.
 */
stack *stack_push(stack *stk, const void *const elem);

#ifdef STACK_INCL_NO_HEAP_STACK
/* Ensures that each stack's allocation gets a fairly unique name. */
#define GET_STACK_NAME(local_stk) _stk_data_##local_stk

#define GET_STACK_ALLOC_SIZE(num_elems, elem_size) \
  ((num_elems) * (elem_size) + sizeof(stack))

/*
 * Creates a stack with automatic storage duration.
 *
 * \param stk_id The identifer for the stack being assigned.
 * \param num_elems The maximum number of elements the stack will contain.
 * \param _elem_size The size of each element in the stack.
 */
#define new_stack_no_heap(stk_id, num_elems, _elem_size)                      \
  {.capacity = (num_elems) * (_elem_size),                                    \
   .used_capacity = 0,                                                        \
   .elem_size = _elem_size,                                                   \
   .length = 0};                                                              \
  byte_t GET_STACK_NAME(stk_id)[GET_STACK_ALLOC_SIZE(num_elems, _elem_size)]; \
  (stk_id).data = GET_STACK_NAME(stk_id) + sizeof(stk_id)

/*
 * Creates a stack of automatic storage duration which allocates memory solely
 * for the stack header (that is, the data members of `stack`). `stack->data`
 * will point to `data`.
 *
 * \note This stack will not modify the allocation of memory at `data`,
 * however it can modify the contents of `data` through `no_heap_stack_pop()`
 * and `no_heap_stack_push()`.
 */
#define stack_interface(_data, num_elems, _elem_size) \
  {.data = _data,                                     \
   .capacity = (num_elems) * (_elem_size),            \
   .used_capacity = (num_elems) * (_elem_size),       \
   .elem_size = _elem_size,                           \
   .length = num_elems}

/*
 * Returns, but does not remove, the top element of `stk`.
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
 * Adds `elem` to `stk` if space permits. `elem` will then be the new top
 * element and will be returned by functions such as `no_heap_stack_peek()`.
 *
 * \return `stk` if the element was added successfully or `NULL` upon failure.
 */
stack *no_heap_stack_push(stack *stk, const void *const elem);
#endif
#endif
