#include "tree.h"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

tree_t *_new_tree(const void *const data, const size_t elem_size,
                  const size_t length) {
  const size_t NODE_SIZE = sizeof(node_t) + elem_size;
  const size_t REQUIRED_MEM = length * NODE_SIZE + sizeof(tree_t);
  tree_t *const tree_obj = malloc(REQUIRED_MEM);
  if (tree_obj == NULL) return NULL;

  /* Adding one since the root node is counted in the depth. */
  tree_obj->depth = length / 2 + 1;
  tree_obj->num_nodes = length;
  tree_obj->bytes_allocated = REQUIRED_MEM;
  tree_obj->bytes_used = REQUIRED_MEM;

  node_t *nodes_heap = (node_t *)((char *)tree_obj + sizeof(tree_t));
  nodes_heap->parent = NULL;
  nodes_heap->value = (char *)nodes_heap + sizeof(node_t);
  memcpy(nodes_heap->value, data, elem_size);
  tree_obj->root = nodes_heap;
  for (size_t i = 1; i < length; i++) {
    nodes_heap += NODE_SIZE;
    /* This expression exists to give the tree the structure shown in the
     * documentation's example. */
    node_t *const parent_node =
        (node_t *)((char *)nodes_heap - (i & 1 ? NODE_SIZE : 2 * NODE_SIZE));
    nodes_heap->parent = parent_node;
    nodes_heap->value = (char *)nodes_heap + sizeof(node_t);
    memcpy(nodes_heap->value, (char *)data + i * elem_size, elem_size);
    printf("%d\n", (int)(*(int *)parent_node->value));
  }
  return tree_obj;
}

int main(void) {
  int data[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
  tree_t *a = new_tree(data, sizeof(data) / sizeof(*data));
  printf("%d\n", (int)(*(int *)a->root->value));
  putchar('a');
  return 0;
}
