/*
 * Define this in your source files above `#include "stack.h"` to expose some
 * functionality allowing the use of stacks without worry of dynamic memory.
 */
#define STACK_INCL_HEAPLESS_STACK
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
    stack *stk_from_arr = _stack_from_arr(data, DATA_LEN, sizeof(*data));
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

    const size_t INITIAL_NUM_ELEMS = 5;
    stack *stk_no_init_data = new_stack(INITIAL_NUM_ELEMS, sizeof(*data));
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
   * `1 (N/A: val is NULL)`
   */
  {
    const int data[] = {1, 2, 3, 4, 5};
    stack *example_stk = stack_from_arr(data);

    /* Print the top value of `example_stk`. */
    int *val = stack_peek(example_stk);
    if (val == NULL) return 1;
    printf("%d ", *val);

    clear_stack(example_stk);

    /* Attempt to print the top value of `example_stk` again. */
    val = stack_peek(example_stk);
    if (val == NULL) printf("(N/A: val is NULL)");

    delete_stack(&example_stk);
    putchar('\n');
  }

  /*
   * Example usage of `resize_stack()`.
   *
   * This snippet could output:
   *
   * `4 1600 20 1300 64`
   */
  {
    const size_t ELEM_SIZE = sizeof(int);
    size_t num_elems = 1;
    stack *example_stk = new_stack(num_elems, ELEM_SIZE);
    if (example_stk == NULL) return 1;

    /* Initial capacity. */
    printf("%zu ", example_stk->capacity);

    /* Upscaling by a factor of `example_stk->elem_size`. */
    num_elems = 400;
    example_stk = resize_stack(example_stk, ELEM_SIZE * num_elems);
    if (example_stk == NULL) return 1;
    printf("%zu ", example_stk->capacity);

    /* Downscaling by a factor of `example_stk->elem_size`. */
    num_elems = 5;
    example_stk = resize_stack(example_stk, ELEM_SIZE * num_elems);
    if (example_stk == NULL) return 1;
    printf("%zu ", example_stk->capacity);

    /* Upscaling by a non-factor of `example_stk->elem_size`. */
    num_elems = 100;
    example_stk = resize_stack(example_stk, 13 * num_elems);
    if (example_stk == NULL) return 1;
    printf("%zu ", example_stk->capacity);

    /* Downscaling by a non-factor of `example_stk->elem_size`. */
    num_elems = 9;
    example_stk = resize_stack(example_stk, 7 * num_elems);
    if (example_stk == NULL) return 1;
    printf("%zu", example_stk->capacity);

    delete_stack(&example_stk);
    putchar('\n');
  }

  /*
   * Example usage of `shrink_stack_to_fit()`.
   *
   * This snippet could output:
   *
   * `40 12`
   */
  {
    const size_t MAX_NUM_ELEMS = 10;
    const size_t ELEM_SIZE = sizeof(int);
    stack *example_stk = new_stack(MAX_NUM_ELEMS, ELEM_SIZE);
    if (example_stk == NULL) return 1;

    /* Initial capacity. */
    printf("%zu ", example_stk->capacity);

    const int RANDOM_VAL_1 = 3;
    example_stk = stack_push(example_stk, &RANDOM_VAL_1);
    const int RANDOM_VAL_2 = 932;
    example_stk = stack_push(example_stk, &RANDOM_VAL_2);
    const int RANDOM_VAL_3 = 0xD00D;
    example_stk = stack_push(example_stk, &RANDOM_VAL_3);

    example_stk = shrink_stack_to_fit(example_stk);
    if (example_stk == NULL) return 1;

    printf("%zu", example_stk->capacity);

    delete_stack(&example_stk);
    putchar('\n');
  }

  /*
   * Example usage of `new_heapless_interface_stack()`.
   *
   * This snippet should output:
   *
   * `5 4 3 2 1`
   */
  {
    int data[] = {1, 2, 3, 4, 5};
    stack interface_stk = new_heapless_interface_stack(
        data, sizeof(data) / sizeof(*data), sizeof(*data));

    int *val;
    while ((val = interface_stack_pop(&interface_stk))) {
      printf("%d ", *val);
    }

    putchar('\n');
    /*
     * Deletion is unnecessary since `interface_stk` was not allocated on the
     * heap.
     */
  }

  /*
   * Example usage of `new_stack_no_heap()`
   *
   * This snippet should output:
   *
   * `4 3 2 1 0 `
   */
  {
    const size_t NUM_ELEMS = 5;
    stack heapless_stk =
        heapless_new_stack(heapless_stk, NUM_ELEMS, sizeof(int));

    /* Push values onto the stack until capacity is reached. */
    for (size_t i = 0; i < NUM_ELEMS; i++) {
      heapless_stack_push(&heapless_stk, &i);
    }

    /*
     * This push should not succeed since the stack is full and cannot be
     * resized.
     */
    const int RANDOM_VAL_1 = -1;
    heapless_stack_push(&heapless_stk, &RANDOM_VAL_1);

    /* Pop and print all of the values in the stack. */
    int *val;
    while ((val = heapless_stack_pop(&heapless_stk)) != NULL) {
      printf("%d ", *val);
    }
    putchar('\n');
    /*
     * Deletion is unnecessary since `interface_stk` was not allocated on the
     * heap.
     */
  }
  return 0;
}
