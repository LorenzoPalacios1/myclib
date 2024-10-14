/*
 * Define this to expose some functionality allowing the use of stacks without
 * worry of dynamic memory.
 */
#define STACK_INCL_NO_HEAP_STACK
#include <stdio.h>

#include "stack.h"

int main(void) {
  /*
   * Example usage of `_stack_from_arr()`.
   *
   * This snippet should output:
   *
   * `1 2 3 4 5 `
   */
  {
    const int data[] = {1, 2, 3, 4, 5};
    const size_t DATA_LEN = sizeof(data) / sizeof(*data);
    const size_t DATA_ELEM_SIZE = sizeof(*data);
    stack *stk_from_arr = _stack_from_arr(data, DATA_LEN, DATA_ELEM_SIZE);
    if (stk_from_arr == NULL) return 1;

    /* Printing and popping all of the values in `stk_from_arr`. */

    int *val = stack_pop(stk_from_arr);
    while (val != NULL) {
      printf("%d ", *val);
      val = stack_pop(stk_from_arr);
    }

    delete_stack(&stk_from_arr);
    putchar('\n');
  }

  /*
   * Example usage of `stack_from_arr()`, a convenience macro over
   * `_stack_from_arr()`.
   *
   * This snippet should output:
   *
   * `1 2 3 4 5 `
   */
  {
    const int data[] = {1, 2, 3, 4, 5};
    stack *stk_from_arr_macro = stack_from_arr(data);
    if (stk_from_arr_macro == NULL) return 1;

    /* Printing and popping all of the values in `stk_from_arr_macro`. */

    int *val = stack_pop(stk_from_arr_macro);
    while (val != NULL) {
      printf("%d ", *val);
      val = stack_pop(stk_from_arr_macro);
    }

    delete_stack(&stk_from_arr_macro);
    putchar('\n');
  }

  /*
   * Example usage of `new_stack()`.
   *
   * This snippet should output:
   *
   * `2147483647 1 0 5 4 3 2 1 `
   */
  {
    const int data[] = {1, 2, 3, 4, 5};
    const size_t DATA_LEN = sizeof(data) / sizeof(*data);
    const size_t DATA_ELEM_SIZE = sizeof(*data);

    const size_t INITIAL_NUM_ELEMS = 5;
    stack *stk_no_init_data = new_stack(INITIAL_NUM_ELEMS, DATA_ELEM_SIZE);
    if (stk_no_init_data == NULL) return 1;

    /* Pushing the elements of `data` to `stk_no_init_data`. */
    for (size_t i = 0; i < DATA_LEN; i++) {
      stk_no_init_data = stack_push(stk_no_init_data, data + i);
      if (stk_no_init_data == NULL) return 1; /* Reallocation could fail. */
    }

    /* Pushing arbitrary values to `stk_no_init_data`. */

    const int RANDOM_VAL_1 = 0; /* 32-bit `signed int` maximum value */
    stk_no_init_data = stack_push(stk_no_init_data, &RANDOM_VAL_1);
    if (stk_no_init_data == NULL) return 1;

    const int RANDOM_VAL_2 = 1;
    stk_no_init_data = stack_push(stk_no_init_data, &RANDOM_VAL_2);
    if (stk_no_init_data == NULL) return 1;

    /*
     * Since this is the last element to be pushed onto `stk_no_init_data`, it
     * will be the first to be popped.
     */
    const int RANDOM_VAL_3 = 0x7FFFFFFF;
    stk_no_init_data = stack_push(stk_no_init_data, &RANDOM_VAL_3);
    if (stk_no_init_data == NULL) return 1;

    /* Printing and popping all of the values in `stk_no_init_data`. */

    int *val = stack_pop(stk_no_init_data);
    while (val != NULL) {
      printf("%d ", *val);
      val = stack_pop(stk_no_init_data);
    }

    delete_stack(&stk_no_init_data);
    putchar('\n');
  }

  /*
   * Example usage of `clear_stack()`.
   *
   * This snippet should output:
   *
   * `1 (N/A: NULL dereference)`
   */
  {
    const int data[] = {1, 2, 3, 4, 5};
    stack *example_stack = stack_from_arr(data);

    /* Print the top value of `example_stack`. */
    int *val = stack_peek(example_stack);
    if (val == NULL) return 1;
    printf("%d ", *val);

    clear_stack(example_stack);

    /* Attempt to print the top value of `example_stack` again. */
    val = stack_peek(example_stack);
    if (val == NULL) printf("(N/A: val is NULL)");

    delete_stack(&example_stack);
    putchar('\n');
  }

  /*
   * Example usage of `resize_stack()`.
   *
   * This snippet could output:
   *
   * 4 1600 36 1300
   */
  {
    const size_t elem_size = sizeof(int);
    size_t num_elems = 1;
    stack *example_stack = new_stack(num_elems, elem_size);
    if (example_stack == NULL) return 1;

    /* Initial capacity. */
    printf("%zu ", example_stack->capacity);

    /* Upscaling by a factor of `example_stack->elem_size`. */
    num_elems = 400;
    example_stack = resize_stack(example_stack, elem_size * num_elems);
    if (example_stack == NULL) return 1;
    /* New, greater capacity*/
    printf("%zu ", example_stack->capacity);

    /* Downscaling by a factor of `example_stack->elem_size`. */
    num_elems = 5;
    example_stack = resize_stack(example_stack, 7 * num_elems);
    if (example_stack == NULL) return 1;
    printf("%zu ", example_stack->capacity);

    /* Upscaling by a non-factor of `example_stack->elem_size`. */
    num_elems = 100;
    example_stack = resize_stack(example_stack, 13 * num_elems);
    if (example_stack == NULL) return 1;
    printf("%zu ", example_stack->capacity);

    /* Downscaling by a non-factor of `example_stack->elem_size`. */
  }

  return 0;
}
