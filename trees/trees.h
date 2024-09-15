#ifndef TREES_H
#define TREES_H

#include <stddef.h>

#include "binarytree/binarytree.h"

#define delete_tree(tree) \
  (_Generic(&(tree), binary_tree_t *: delete_binary_tree)(&(tree)))
#define delete_tree_s(tree) \
  (_Generic(&(tree), binary_tree_t *: delete_binary_tree_s)(&(tree)))
#define add_node(tree, node) \
  (_Generic(tree, binary_tree_t *: add_binary_node)(tree, node))
#define remove_node(tree, node) \
  (_Generic(tree, binary_tree_t *: remove_binary_node)(tree, node))

#endif
