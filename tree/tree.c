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
   * the minimum amount of memory required to contain a list of pointers to each
   * node's children.
   *
   * `length - 1` exists in the second expression since the root node will have
   * at most two children, while every other node will have at most one, except
   * for the last two, which will have no children. This gives the expression:
   *
   * `(n + 1 - 2) * ptr_size`
   *
   * where `n` is the number of nodes.
   */
  const size_t REQUIRED_MEM =
      (length * NODE_SIZE + sizeof(tree_t)) + ((length - 1) * sizeof(node_t *));
  tree_t *const tree_obj = malloc(REQUIRED_MEM);
  if (tree_obj == NULL) return NULL;

  tree_obj->num_nodes = length;
  tree_obj->bytes_allocated = REQUIRED_MEM;
  tree_obj->bytes_used = REQUIRED_MEM;

  node_t *nodes_mem = (node_t *)((char *)tree_obj + sizeof(tree_t));
  node_t **children_mem = (node_t **)((char *)tree_obj + REQUIRED_MEM -
                                      ((length - 1) * sizeof(node_t *)));
  /* Root node initialization. */
  nodes_mem[0].parent = NULL;
  nodes_mem[0].num_children = 0;
  nodes_mem[0].value = nodes_mem + 1;
  memcpy(nodes_mem[0].value, data, elem_size);
  tree_obj->root = nodes_mem;
  nodes_mem = (node_t *)((char *)nodes_mem + NODE_SIZE);
  for (size_t i = 1; i < length; i++) {
    nodes_mem->num_children = 0;
    nodes_mem->value = nodes_mem + 1;
    memcpy(nodes_mem->value, (char *)data + i * elem_size, elem_size);
    /*
     * This ensures the root node is the parent to two nodes and that all other
     * nodes are the parent to no more than one node.
     */
    node_t *parent_node;
    if (i <= 2)
      parent_node = tree_obj->root;
    else
      parent_node = (node_t *)((char *)nodes_mem - 2 * NODE_SIZE);
    nodes_mem->parent = parent_node;
    children_mem[0] = nodes_mem;
    /*
     * This statement ensures the third node becomes a child of the tree's root
     * node. Until I better revise my design, this ugly hack will have to stay.
     */
    if (i != 2) parent_node->children = children_mem;
    parent_node->num_children++;

    nodes_mem = (node_t *)((char *)nodes_mem + NODE_SIZE);
    children_mem++;
  }
  return tree_obj;
}

void _delete_tree(tree_t **const tree) {
  free(*tree);
  *tree = NULL;
}

void _secure_delete_tree(tree_t **const tree) {
  memset(*tree, 0, (*tree)->bytes_allocated);
  _delete_tree(tree);
}

node_t *remove_node(tree_t *const src, node_t *const target) {
  node_t *const target_parent = target->parent;
  printf("%d", target_parent->children[0]->value);
  for (size_t i = 0; i < target_parent->num_children; i++) {
    if (target_parent->children[i] == target) {
      memcpy(&target->parent->children[i], &target->parent->children[i + 1],
             target_parent->num_children - i);
    }
  }
  src->num_nodes -= target->num_children + 1;
  target_parent->num_children--;
  target->parent = NULL;
  return target;
}

int main(void) {
  static const int data[] = {1, 2, 3, 4, 5, 6};
  tree_t *a = new_tree(data, sizeof data / sizeof *data);
  printf("%d\n", *(int *)a->root->children[0]->value);
  return 0;
}
