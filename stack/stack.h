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

/* Clears the elements of `stk` and resets its `length` to `0`. */
void clear_stack(stack *stk);

/*
 * Same as `clear_stack()`, except this function will also zero the memory
 * associated with `stk->data`.
 */
void clear_stack_s(stack *stk);

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
 * Adds `elem` to `stk`, expanding if necessary. `elem` will then be the new top
 * element and will be returned by functions such as `stack_peek()`.
 *
 * \return A pointer associated with the contents of `stk` or `NULL` upon
 * failure.
 */
stack *stack_push(stack *stk, const void *const elem);

#ifdef STACK_INCL_NO_HEAP_STACK
/* Ensures that each stack's allocation gets a fairly unique name. */
#define STACK_NAME(local_stk) _stk_data_##local_stk

#define GET_STACK_ALLOC_SIZE(num_elems, elem_size) \
  ((num_elems) * (elem_size) + sizeof(stack))
/*
 * Creates a stack with automatic storage duration at `local_stk`.
 *
 * \param local_stk A pointer to a local variable of type `stack`.
 * \param num_elems The maximum number of elements the stack will contain.
 * \param _elem_size The size of each element in the stack.
 */
#define new_stack_no_heap(local_stk, num_elems, _elem_size)                  \
  byte_t STACK_NAME(local_stk)[GET_STACK_ALLOC_SIZE(num_elems, _elem_size)]; \
  (local_stk) = (void *)STACK_NAME(local_stk);                               \
  (*(local_stk)).data = local_stk + 1;                                       \
  (*(local_stk)).capacity = num_elems * _elem_size;                          \
  (*(local_stk)).used_capacity = 0;                                          \
  (*(local_stk)).elem_size = _elem_size;                                     \
  (*(local_stk)).length = 0

/*
 * Creates a stack of automatic storage duration which allocates memory solely
 * for the stack header (that is, the data members of `stack`). `stack->data`
 * will point to `data`.
 *
 * \note This stack will not modify the allocation of memory at `data`, however
 * it can modify the contents of `data` through `no_heap_stack_pop()` and
 * `no_heap_stack_push()`.
 */
#define stack_interface(data, num_elems, _elem_size)

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
 * Adds `elem` to `stk` if space permits. `elem` will then be the new top
 * element and will be returned by functions such as `no_heap_stack_peek()`.
 *
 * \return `stk` if the element was added successfully or `NULL` upon failure.
 */
stack *no_heap_stack_push(stack *stk, const void *const elem);
#endif
#endif
