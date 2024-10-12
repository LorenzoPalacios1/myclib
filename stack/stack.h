#ifndef STACK_H
#define STACK_H

#include <stddef.h>

#include "bwdstack/bwdstack.h"

typedef struct {
  void *data;
  size_t elem_size;
} stack;

#define delete_stack(stk) (_Generic(stk, bwd_stack *: delete_bwd_stack)(stk))

#endif
