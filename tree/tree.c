#include "tree.h"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

tree_t *_new_tree(const void *const data, const size_t elem_size,
                  const size_t length) {
  const size_t NODE_SIZE = sizeof(node_t) + elem_size;
  /*
   * The first portion of the below expression determines the minimum amount of
   * memory required for the tree and its nodes. The second portion determines
   * the minimum amount of memory required to contain a list of each node's
   * children.
   *
   * `length - 1` exists in the second expression since the root node will have
   * two children, while every other node will have only one. Meanwhile, the
   * last two will have no children. This gives the unmodified expression
   *
   * (n + 1 - 2) * ptr_size
   */
  const size_t REQUIRED_MEM =
      (length * NODE_SIZE + sizeof(tree_t)) + ((length - 1) * sizeof(node_t *));
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
    nodes_heap = (node_t *)((char *)nodes_heap + NODE_SIZE);
    /*
     * This expression exists to give the nodes the structure shown in the
     * documentation's example. The ensures the root node is the parent to two
     * nodes, after which all other nodes are the parent to at most one node.
     */
    node_t *const parent_node =
        (node_t *)((char *)nodes_heap - (i == 2 ? 2 * NODE_SIZE : NODE_SIZE));
    nodes_heap->parent = parent_node;
    nodes_heap->value = (char *)nodes_heap + sizeof(node_t);
    memcpy(nodes_heap->value, (char *)data + i * elem_size, elem_size);
  }
  return tree_obj;
}

int main(void) {
  int data[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
  tree_t *a = new_tree(data, sizeof(data) / sizeof(*data));
  printf("%d\n", (int)(*(int *)a->root->value));
  return 0;
}
