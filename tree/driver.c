#include <stdio.h>

#include "tree.h"

int main(void) {
  const int data[] = {10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0};
  const tree_t *a = create_tree(data, sizeof(data) / sizeof(int), sizeof(int));

  printf("%llu", a->max_depth);

  return 0;
}
