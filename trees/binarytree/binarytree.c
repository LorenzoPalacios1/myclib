#include "binarytree.h"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

binary_tree_t *_new_binary_tree(const void *const data, const size_t elem_size,
                                const size_t length) {
  const size_t NODE_SIZE = sizeof(binary_node_t) + elem_size;
  const size_t REQUIRED_MEM = length * NODE_SIZE + sizeof(binary_tree_t);
  binary_tree_t *const tree_obj = malloc(REQUIRED_MEM);
  if (tree_obj == NULL) return NULL;

  tree_obj->num_nodes = length;
  tree_obj->bytes_allocated = REQUIRED_MEM;
  tree_obj->bytes_used = REQUIRED_MEM;

  binary_node_t *const nodes_mem =
      (binary_node_t *)((char *)tree_obj + sizeof(binary_tree_t));
  for (size_t i = 0; i < length; i++) {
    binary_node_t *const cur_node = nodes_mem + i * NODE_SIZE;
    binary_node_t *parent_node;
    if (i == 0)
      parent_node = NULL;
    else if (i == 1)
      parent_node = cur_node - NODE_SIZE;
    else
      parent_node = cur_node - NODE_SIZE * (i & 1);
    3 & 1;
    char *const cur_node_data = (char *)nodes_mem + sizeof(binary_node_t);
    memcpy(cur_node_data, (char *)data + i * elem_size, elem_size);
    nodes_mem->value = cur_node_data;
  }
  tree_obj->root = nodes_mem; /* The first node is always the root node. */
  return tree_obj;
}

void delete_binary_tree(binary_tree_t **const tree) {
  free(*tree);
  *tree = NULL;
}

void delete_binary_tree_s(binary_tree_t **const tree) {
  memset(*tree, 0, (*tree)->bytes_allocated);
  _delete_tree(tree);
}

binary_node_t *unparent_node(binary_tree_t *const src,
                             binary_node_t *const target) {}

int main(void) {
  static const int data[] = {1, 2, 3, 4, 5, 6};
  binary_tree_t *a = new_tree(data, sizeof data / sizeof *data);
  printf("%d\n", *(int *)a->root->children[0]->value);
  remove_node(a, a->root->children[0]);
  printf("%d\n", *(int *)a->root->children[0]->value);
  return 0;
}
