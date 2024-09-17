#include "binarytree.h"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../trees.h"

binary_tree_t *_new_binary_tree(const void *const data, const size_t elem_size,
                                const size_t length) {
  const size_t NODE_SIZE = sizeof(binary_node_t) + elem_size;
  const size_t REQUIRED_MEM = length * NODE_SIZE + sizeof(binary_tree_t);
  binary_tree_t *const tree_obj = malloc(REQUIRED_MEM);
  binary_node_t *const nodes_mem = (void *)(tree_obj + 1);
  if (tree_obj == NULL) return NULL;
  tree_obj->num_nodes = length;
  tree_obj->bytes_allocated = REQUIRED_MEM;
  tree_obj->bytes_used = REQUIRED_MEM;
  for (size_t i = 0; i < length; i++) {
    binary_node_t *const cur_node = (void *)((char *)nodes_mem + i * NODE_SIZE);
    binary_node_t *parent_node;
    void *const cur_node_data = (char *)cur_node + sizeof(binary_node_t);
    if (i == 0)
      parent_node = NULL;
    else {
      /* Ensures all odd nodes become the left child of `parent_node`. */
      parent_node = (void *)((char *)cur_node - NODE_SIZE * ((~i & 1) + 1));
      parent_node->children[~i & 1] = cur_node;
    }
    memcpy(cur_node_data, (char *)data + i * elem_size, elem_size);
    cur_node->value = cur_node_data;
    cur_node->parent = parent_node;
    cur_node->children[0] = cur_node->children[1] = NULL;
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
  delete_binary_tree(tree);
}

binary_node_t *unparent_node(binary_tree_t *const src,
                             binary_node_t *const target);

int main(void) {
  static const int data[] = {1, 2, 3, 4, 5, 6};
  binary_tree_t *a = new_binary_tree(data, sizeof data / sizeof *data);
  printf("%d\n", *(int *)a->root->value); // 1 (works)
  printf("%d\n", *(int *)a->root->children[0]->value); // 2 (works)
  printf("%d\n", *(int *)a->root->children[1]->value); // 3 (works)
  printf("%d\n", *(int *)a->root->children[0]->children[0]->value); // 4
  printf("%d\n", *(int *)a->root->children[0]->children[1]->value); // 5
  printf("%d\n", *(int *)a->root->children[1]->children[0]->value); // 6

  return 0;
}
