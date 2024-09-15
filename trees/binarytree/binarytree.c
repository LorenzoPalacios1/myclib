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

  binary_node_t *nodes_mem =
      (binary_node_t *)((char *)tree_obj + sizeof(binary_tree_t));
  /* Root node initialization. */
  nodes_mem[0].parent = NULL;
  nodes_mem[0].value = nodes_mem + 1;
  memcpy(nodes_mem[0].value, data, elem_size);
  tree_obj->root = nodes_mem;
  nodes_mem = (binary_node_t *)((char *)nodes_mem + NODE_SIZE);
  for (size_t i = 1; i < length; i++) {
    nodes_mem->value = (char *)nodes_mem + sizeof(binary_node_t);
    memcpy(nodes_mem->value, (char *)data + i * elem_size, elem_size);
    

    nodes_mem = (binary_node_t *)((char *)nodes_mem + NODE_SIZE);
  }
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
                             binary_node_t *const target) {
}

int main(void) {
  static const int data[] = {1, 2, 3, 4, 5, 6};
  binary_tree_t *a = new_tree(data, sizeof data / sizeof *data);
  printf("%d\n", *(int *)a->root->children[0]->value);
  remove_node(a, a->root->children[0]);
  printf("%d\n", *(int *)a->root->children[0]->value);
  return 0;
}
