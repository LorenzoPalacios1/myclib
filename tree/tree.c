#include "tree.h"

#include <stdlib.h>

#define isOdd(a) ((a) & 1)

tree_t *create_tree(const void *const arr, const size_t arr_length,
                    const size_t elem_size_in_bytes) {
  tree_t *const new_tree = malloc(sizeof(tree_t));
  node_t *const nodes_heap = malloc(arr_length * sizeof(node_t));
  char *const node_values_heap = malloc(arr_length * elem_size_in_bytes);

  for (size_t i = 0; i < elem_size_in_bytes; i++)
    node_values_heap[i] = ((char *)arr)[i];

  new_tree->root_node = &nodes_heap[0];
  new_tree->root_node->node_value = &node_values_heap[0];
  new_tree->root_node->num_children = 0;
  new_tree->root_node->parent = NULL;

  for (size_t cur_node = 1; cur_node < arr_length; cur_node++) {
    const size_t ARR_I_BEGIN = cur_node * elem_size_in_bytes;
    const size_t ARR_I_END = (cur_node + 1) * elem_size_in_bytes;
    for (size_t arr_i = ARR_I_BEGIN; arr_i < ARR_I_END; arr_i++)
      node_values_heap[arr_i] = ((char *)arr)[arr_i];
    /*
     * For every odd node, its parent will be the node before the previous.
     * For every even node, its parent will be the previous node.
     */
    const unsigned char parent_assignment_offset = isOdd(cur_node) + 1;
    node_t *parent_node = &nodes_heap[cur_node - parent_assignment_offset];
    nodes_heap[cur_node].node_value = &node_values_heap[cur_node];
    nodes_heap[cur_node].parent = parent_node;
  }
  new_tree->num_nodes = arr_length;
  new_tree->size_of_node_value_in_bytes = elem_size_in_bytes;
  /* The root node is considered a part of the tree's depth, hence `+ 1`. */
  new_tree->max_depth = arr_length / 2 + 1;

  return new_tree;
}

static bool delete_tree_recursive(node_t *cur_node) {
  if (cur_node->num_children == 0) {
    free(cur_node->node_value);
    free(cur_node->children);
    return true;
  }
  return delete_tree_recursive(&(cur_node->children[0]));
}

static node_t *get_next_child_node(const node_t *const base_node) {
  node_t *const children = base_node->children;
  for (size_t i = 0; i < base_node->num_children; i++) {
    if (&(children[i]) != NULL) return &(children[i]);
  }
  return NULL;
}

/* This is an internal function used by `delete_tree`. */
static bool delete_node(node_t *node_to_delete) {}
