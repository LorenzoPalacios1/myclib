#include "binarytree.h"

#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include "../trees.h"

binary_tree *_new_binary_tree(const void *const data, const size_t elem_size,
                              const size_t length) {
  const size_t NODE_SIZE = sizeof(bt_node) + elem_size;
  const size_t REQUIRED_MEM = length * NODE_SIZE + sizeof(binary_tree);
  binary_tree *const tree_obj = malloc(REQUIRED_MEM);
  /* Increment past the tree's data. */
  bt_node *const nodes_mem = (void *)(tree_obj + 1);
  if (tree_obj == NULL) return NULL;

  tree_obj->num_nodes = length;
  tree_obj->node_size = NODE_SIZE;
  tree_obj->used_allocation = tree_obj->allocation = REQUIRED_MEM;

  for (size_t i = 0; i < length; i++) {
    bt_node *const cur_node = (void *)((char *)nodes_mem + i * NODE_SIZE);
    bt_node *parent_node;
    if (i == 0)
      parent_node = NULL;
    else {
      parent_node = (void *)((char *)cur_node - NODE_SIZE * (i / 2 + 1));
      if (i & 1)
        parent_node->left = cur_node;
      else
        parent_node->right = cur_node;
    }
    void *const cur_node_data = (char *)cur_node + sizeof(bt_node);
    memcpy(cur_node_data, (char *)data + i * elem_size, elem_size);
    cur_node->value = cur_node_data;
    cur_node->parent = parent_node;
    cur_node->left = cur_node->right = NULL;
  }
  tree_obj->root = nodes_mem; /* The first node is always the root node. */
  return tree_obj;
}

void delete_binary_tree(binary_tree **const tree) {
  free(*tree);
  *tree = NULL;
}

void delete_binary_tree_s(binary_tree **const tree) {
  memset(*tree, 0, (*tree)->allocation);
  delete_binary_tree(tree);
}

bt_node *remove_node_from_tree(binary_tree *const tree, bt_node *target) {
  bt_node *removed_copy = malloc(tree->node_size);
  if (removed_copy == NULL) return NULL;
  if (target->parent->left == target)
    target->parent->left = NULL;
  else if (target->parent->right == target)
    target->parent->right = NULL;
  tree->used_allocation -= tree->node_size;
  memcpy(removed_copy, target, tree->node_size);
  return removed_copy;
}

void delete_node_and_ancestry(bt_node *target) {

}

void delete_node(bt_node *target) {
  bt_node *parent = target->parent;
  bt_node *new_slot = parent->left;
  while (new_slot == NULL) {

  }
}
