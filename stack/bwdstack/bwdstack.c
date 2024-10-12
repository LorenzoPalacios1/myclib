#include "bwdstack.h"

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

bwd_stack *_bwd_stack_from_arr(const void *const data, const size_t len,
                              const size_t elem_size) {
  bwd_stack *const stk = create_bwd_stack(len, elem_size);
  if (stk == NULL) return NULL;
  stk->pos = len - 1;
  /*
   * Since a backwards stack requires its constituent elements to be
   * laid out in reverse order, we iterate from the last element of
   * `data` to its first.
   */
  for (size_t i = 0; i < len; i++) {
    memcpy((char *)stk->data + i * elem_size,
           (char *)data + (len - i * elem_size), elem_size);
  }
  return stk;
}

int main(void) {
  const int data[] = {1, 2, 3, 4};
  bwd_stack *a = bwd_stack_from_arr(data);
  return 0;
}
