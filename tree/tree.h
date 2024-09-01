#ifndef TREE_H
#define TREE_H

#include <stdbool.h>
#include <stddef.h>

typedef struct node_t {
  void *value;
  struct node_t *parent;
  struct node_t **children;
  size_t num_children;
} node_t;

/* Container structure for a generic tree data structure. */
typedef struct tree_t {
  node_t *root;
  size_t num_nodes;
  size_t depth;
  size_t bytes_allocated; /* Total bytes allocated for the tree and nodes. */
  size_t bytes_used;
} tree_t;

#define new_tree(data, length) _new_tree(data, sizeof *(data), length)

#define delete_tree(tree) _delete_tree(&tree)

/*
 * Initializes a tree with the given elements from the passed array.
 * The first element in the array is always the root node, and subsequent
 * array elements are placed as evenly as possible on either side.
 *
 * Ex. Consider an array of five elements: {1, 2, 3, 4, 5}.
 *     The resultant tree would look like this:
 *
 *         1
 *        / \
 *       2   3
 *      /     \
 *     4       5
 */
tree_t *_new_tree(const void *data, size_t elem_size, size_t length);

/*
 * Frees the passed tree's consumed memory and reassigns the pointer to the tree
 * to `NULL`.
 */
void _delete_tree(tree_t **tree_to_delete);

/*
 * Same as `delete_tree()`, except this function will set all allocated memory
 * of the tree to zero, including pointers and tree/node statistics.
 */
bool secure_delete_tree(tree_t *tree_to_delete);

/*
 * Adds the specified element to the passed tree.
 * This function will update the tree's statistics if the element was
 * successfully added to the tree.
 */
bool add_node(tree_t *parent_tree, const void *elem);

/*
 * Removes `target` node from `src`, thereby causing `target` to have no parent
 * (assumed `NULL`).
 *
 * \return The removed node.
 */
node_t *remove_node(tree_t *src, node_t *target);

/*
 * This function takes the first argument and re-parents it to the
 * second argument, including any child nodes with it.
 */
void make_node_child_of(const node_t *node_to_be_moved,
                        const node_t *new_parent);

/*
 * Performs a linear search for the specified value in the passed tree.
 *
 * \return A pointer to the first node containing the desired value, or `NULL`
 * upon failure.
 */
node_t *search_for_elem(const tree_t *tree_to_be_searched, const void *elem);

/*
 * Updates the statistics for the passed tree.
 */
void update_tree_stats(const tree_t *tree_to_be_updated);
#endif
