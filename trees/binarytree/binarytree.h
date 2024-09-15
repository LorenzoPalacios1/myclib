#ifndef BINARYTREE_H
#define BINARYTREE_H

#include <stddef.h>

typedef struct binary_node_t {
  void *value;
  struct binary_node_t *parent;
  /* `children[0]` is the left node and `children[1]` is the right node. */
  struct binary_node_t *children[2];
} binary_node_t;

/* Container structure for a binary tree data structure. */
typedef struct binary_tree_t {
  binary_node_t *root;
  size_t num_nodes;
  size_t bytes_allocated; /* Total bytes allocated for the tree and nodes. */
  size_t bytes_used;
} binary_tree_t;

/*
 * This is a convenience macro for generating a `binary_tree_t` from an array.
 * Make sure the arguments have no side effects such as incrementation.
 */
#define new_binary_tree(data, length) \
  _new_binary_tree(data, sizeof *(data), length)

/*
 * Initializes a binary tree with the given elements from the passed array.
 * The first element in the array is always the root node, and subsequent
 * array elements are placed as evenly as possible on either side.
 *
 * Ex. Consider an array of seven elements: {7, 4, 9, 3, 0, 1, 0}.
 *     The resultant tree would look like this:
 *
 *         7
 *       /   \
 *      4     9
 *     / \   / \
 *    3   1 0   0
 */
binary_tree_t *_new_binary_tree(const void *data, size_t elem_size,
                                size_t length);

/*
 * Frees the passed binary tree's consumed memory and reassigns its pointer
 * to `NULL`.
 */
void delete_binary_tree(binary_tree_t **tree);

/*
 * Same as `delete_binary_tree()`, except this function will set all allocated
 * memory of the tree to zero, including pointers and tree/node statistics.
 */
void delete_binary_tree_s(binary_tree_t **tree);

/*
 * Adds the specified element as a child node of `parent_node`.
 * \return A pointer to the added node, or NULL upon failure.
 */
binary_node_t *add_binary_node(binary_tree_t *tree, const void *elem);

/*
 * Removes `target` from `src`, thereby causing `target` to have no parent
 * (assumed `NULL`).
 *
 * \return The removed node.
 */
binary_node_t *remove_binary_node(binary_tree_t *src, binary_node_t *target);

/*
 * This function makes `child` a child node of `parent`, including any child
 * nodes.
 */
void reparent_binary_node(const binary_node_t *parent, const binary_node_t *child);
#endif
