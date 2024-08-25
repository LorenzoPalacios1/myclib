#include "tree.h"

#include <stdlib.h>

tree_t *_new_tree(const void *const data, const size_t elem_size,
                  const size_t length) {
  const size_t NUM_NODES = elem_size * length * sizeof(node_t);
  tree_t *const new_tree = malloc(NUM_NODES + sizeof(tree_t));

  /* Adding one since the root node is counted in the depth. */
  new_tree->depth = length / 2 + 1;
  new_tree->num_nodes = NUM_NODES;

  node_t *nodes_heap = (node_t *)((char *)new_tree + sizeof(tree_t));

  for (size_t i = 0; i < NUM_NODES; i++) {
    
  }
  return new_tree;
}
