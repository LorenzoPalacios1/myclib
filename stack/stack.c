#include "bwdstack.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static bwd_stack *alloc_bwd_stack(const size_t stack_capacity) {
  return malloc(stack_capacity + sizeof(bwd_stack));
}

bwd_stack *create_bwd_stack(const size_t num_elems, const size_t elem_size) {
  const size_t STACK_CAPACITY = num_elems * elem_size;
  bwd_stack *const stk = alloc_bwd_stack(STACK_CAPACITY);
  if (stk == NULL) return NULL;
  stk->capacity = STACK_CAPACITY;
  stk->elem_size = elem_size;
  stk->data = stk + 1;
  stk->pos = 0;
  return stk;
}

bwd_stack *_bwd_stack_from_arr(const void *const arr, const size_t len,
                               const size_t elem_size) {
  bwd_stack *const stk = create_bwd_stack(len, elem_size);
  if (stk == NULL) return NULL;
  stk->pos = len - 1;
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

int main(void) {
  const int data[] = {1, 2, 3, 4};
  bwd_stack *a = bwd_stack_from_arr(data);
  printf("%zu %zu %zu ", a->capacity, a->elem_size, a->pos);
  printf("%d", ((int *)a->data)[a->pos]);
  return 0;
}
