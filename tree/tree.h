#ifndef _TREE_H
#define _TREE_H

#include <stdbool.h>

typedef struct node_t {
  void *node_value;
  struct node_t *parent;
  struct node_t *children;
  size_t num_children;
  size_t node_depth_level;
} node_t;

/*
 * Container structure for a tree data structure.
 * Contains a pointer to the tree's root node as well as some basic
 * statistics about the tree.
 */
typedef struct tree_t {
  node_t *root_node;
  size_t max_depth;
  double avg_node_depth;

  size_t num_nodes;
  size_t size_of_node_value_in_bytes;

  node_t *highest_value_node;
  node_t *lowest_value_node;
} tree_t;

/*
 * Initializes a tree with the given elements from the passed array.
 * The first element in the array is always the root node, and subsequent
 * array elements are placed as evenly as possible on either side.
 *
 * Ex. Consider an array of five elements: {A, B, C, D, E}.
 *     The resultant tree would look like this:
 *
 *         A
 *        / \
 *       B   C
 *      /     \
 *     D       E
 */
tree_t *create_tree(const void *arr, size_t arr_length,
                    size_t elem_size_in_bytes);

/*
 * Frees the entire passed tree's consumed memory and assigns a NULL
 * pointer value to the root.
 */
void delete_tree(tree_t *tree_to_delete);

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
 * Removes the specified element from the passed tree.
 * This function will update the tree's statistics if the element was
 * successfully added to the tree.
 *
 * Returns `true` if the element was successfully removed, and false
 * otherwise.
 */
node_t remove_node(tree_t *parent_tree, const void *elem);

/*
 * This function takes the first argument and re-parents it to the
 * second argument, including any child nodes with it.
 *
 * Returns `true` if the re-parenting was successful, and false otherwise.
 */
bool make_node_child_of(const node_t *node_to_be_moved,
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
