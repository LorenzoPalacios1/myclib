#include "tree.h"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

tree_t *_new_tree(const void *const data, const size_t elem_size,
                  const size_t length) {
  const size_t NUM_NODES = elem_size * length;
  const size_t NODE_SIZE = sizeof(node_t) + elem_size;
  const size_t REQUIRED_MEM = NUM_NODES * NODE_SIZE + sizeof(tree_t);
  tree_t *const tree_obj = malloc(REQUIRED_MEM);

  /* Adding one since the root node is counted in the depth. */
  tree_obj->depth = length / 2 + 1;
  tree_obj->num_nodes = NUM_NODES;

  node_t *nodes_heap = (node_t *)((char *)tree_obj + sizeof(tree_t));
  nodes_heap[0].parent = NULL;
  nodes_heap[0].value = (char *)&nodes_heap[0] + sizeof(node_t);
  memcpy(nodes_heap[0].value, (char *)data, elem_size);
  tree_obj->root = &nodes_heap[0];
  for (size_t i = 1; i < NUM_NODES; i++) {
    node_t *const parent_node = i & 1 ? &nodes_heap[i - 1] : &nodes_heap[i - 2];
    nodes_heap[i].parent = parent_node;
    nodes_heap[i].value = (char *)&nodes_heap[i] + sizeof(node_t);
    memcpy(nodes_heap[i].value, (char *)data + i * elem_size, elem_size);
    printf("%zu\n", i);
  }
  return tree_obj;
}

int main(void) {
  int data[] = {1, 2, 3};
  tree_t *a = new_tree(data, sizeof(data) / sizeof(*data));
  printf("%d", (int)*(int*)(a->root->value));
  return 0;
}
