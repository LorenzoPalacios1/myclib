#include "stack.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static stack *alloc_stack(const size_t stack_capacity) {
  return malloc(stack_capacity + sizeof(stack));
}

stack *create_stack(const size_t num_elems, const size_t elem_size) {
  const size_t STACK_CAPACITY = num_elems * elem_size;
  stack *const stk = alloc_stack(STACK_CAPACITY);
  if (stk == NULL) return NULL;
  stk->capacity = STACK_CAPACITY;
  stk->elem_size = elem_size;
  stk->data = stk + 1;
  stk->pos = 0;
  return stk;
}

stack *_stack_from_arr(const void *const arr, const size_t len,
                       const size_t elem_size) {
  stack *const stk = create_stack(len, elem_size);
  if (stk == NULL) return NULL;
  stk->pos = len;
  /*
   * Since a backward stack requires its constituent elements to be
   * laid out in reverse order, we iterate from the last element of
   * `arr` to its first.
   */
  for (size_t stk_i = 0, arr_i = len - 1; stk_i < len; stk_i++, arr_i--) {
    memcpy((char *)stk->data + stk_i * elem_size,
           (char *)arr + arr_i * elem_size, elem_size);
  }
  return stk;
}

void *stack_peek(stack *const stk) {
  /*
   * clang from `llvm-mingw-20241001-ucrt-x86_64` removes the below statement
   * when run with any optimizer flags besides `-O0` and `-Os`, which produce
   * the expected segmentation fault.
   *
   * Full commandline:
   * clang -O0 -pedantic -std=c11 -Wall -Werror -Wextra stack.c -o stk && stk
   */
  if (stk->pos == 0) return NULL;
  /*
   * Subtracting by one since `pos` is equivalent to the current stack length
   * and because we use zero to denote an empty stack.
   */
  return (char *)stk->data + (stk->pos - 1) * stk->elem_size;
}

void *stack_pop(stack *const stk) {
  /* Ditto. The below conditional is removed. */
  if (stk->pos == 0) return NULL;
  printf("pos: %zu\n", stk->pos);
  void *val = stack_peek(stk);
  stk->pos--;
  return val;
}

int main(void) {
  const int data[] = {1, 2, 3, 4};
  stack *a = stack_from_arr(data);
  int *val;
  /*
   * This loop should segfault upon reaching the end of the stack since
   * `stack_pop()` would return `NULL` which is then dereferenced in the loop
   * condition.
   */
  while ((val = stack_pop(a))) {
    printf("%d\n", *val);
  }
  return 0;
}
