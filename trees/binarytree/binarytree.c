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

/*
 * Finds the longest ancestral path of `origin`. This function searches both the
 * `left` and `right` branches of `origin`.
 *
 * \return The maximum ancestral depth of `origin`.
 */
size_t get_depth(bt_node *const origin) {
  size_t left_branch_depth = 0;
  for (bt_node *cur_node = origin->left; cur_node != NULL;
       left_branch_depth++) {
    if (cur_node->left == NULL)
      cur_node = cur_node->right;
    else
      cur_node = cur_node->left;
  }

  size_t right_branch_depth = 0;
  for (bt_node *cur_node = origin->right; cur_node != NULL;
       right_branch_depth++) {
    if (cur_node->right == NULL)
      cur_node = cur_node->left;
    else
      cur_node = cur_node->right;
  }
  return left_branch_depth > right_branch_depth ? left_branch_depth
                                                : right_branch_depth;
}

/* Calculates the number of ancestor nodes linked to `origin`.

 * \return The total amount of ancestor nodes connected to `origin`.
*/
size_t get_num_ancestral_nodes(bt_node *const origin) {
  size_t count = 0;
  for (bt_node *cur_node = origin->left; cur_node != NULL; count++) {
    if (cur_node->left == NULL)
      cur_node = cur_node->right;
    else
      cur_node = cur_node->left;
  }

  size_t right_branch_depth = 0;
  for (bt_node *cur_node = origin->right; cur_node != NULL; count++) {
    if (cur_node->right == NULL)
      cur_node = cur_node->left;
    else
      cur_node = cur_node->right;
  }
  return count;
}

void delete_node_and_ancestry(binary_tree *const tree, bt_node *target) {
  {
    bt_node *parent = target->parent;
    if (parent->left == target)
      parent->left = NULL;
    else
      parent->right = NULL;
  }
  const size_t REMOVED_NODES = get_depth(target) + 1;
  tree->num_nodes -= REMOVED_NODES;
  tree->used_allocation -= REMOVED_NODES * tree->node_size;
}

void delete_node_and_ancestry_s(binary_tree *const tree, bt_node *target) {
  {
    bt_node *parent = target->parent;
    if (parent->left == target)
      parent->left = NULL;
    else if (parent->right == target)
      parent->right = NULL;
  }

  bt_node *left_node = target->left;
  bt_node *right_node = target->right;
}

/*
 * Searches along the `left` ancestors of `origin` until `target` pointer is
 * found.
 *
 * \return A pointer to `target` or `NULL` if `target` was not found.
 */
static bt_node **node_search_left(bt_node *origin, bt_node *const target) {
  while (origin != NULL) {
    if (origin->left == target) return &origin->left;
    if (origin->right == target) return &origin->right;
    origin = origin->left;
  }
  return NULL;
}

/*
 * Searches along the `right` ancestors of `origin` until `target` pointer is
 * found.
 *
 * \return A pointer to `target` or `NULL` if `target` was not found.
 */
static bt_node **node_search_right(bt_node *origin, bt_node *const target) {
  while (origin != NULL) {
    if (origin->left == target) return &origin->left;
    if (origin->right == target) return &origin->right;
    origin = origin->right;
  }
  return &target->right;
}

/*
 * Finds the first open slot (that is, a `left` or `right` pointer whose value
 * is `NULL`) in a branch of nodes.
 *
 * \return A pointer to an open slot.
 * \note If this function encounters a node whose `left` and `right` pointers
 * are both `NULL`, this function will return a pointer to the `left` pointer.
 */
static bt_node **find_open_ancestor(bt_node *const origin) {
  bt_node **open_slot = node_search_left(origin, NULL);
  if (open_slot == NULL) node_search_right(origin, NULL);
  return open_slot;
}

void make_node_child_of(bt_node *const dst, bt_node *const src) {
  if (dst->left == NULL) {
    dst->left = src;
    return;
  }
  if (dst->right == NULL) {
    dst->right = src;
    return;
  }
}

void delete_node(bt_node *target) { bt_node *parent = target->parent; }
