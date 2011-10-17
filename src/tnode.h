/*! \file
 *
 * \brief A generic strictly binary tree.
 *
 */ 

#ifndef tnode_H_
#define tnode_H_

#include <stdbool.h>
#include "common.h"

// For C++ compilers:
#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief A strictly binary tree.
 */
typedef struct tnode_
{
    char *name; /**< Name of the node. */

    struct tnode_ *p; /**< Pointer to the parent. */

    struct tnode_ *l; /**< Pointer to the 'left' child. */

    struct tnode_ *r; /**< Pointer to the 'right' child. */

    void *data; /**< Data inside the node. */
}
tnode;

/**
 * \brief Initialize a tree object.
 * 
 * \param p       Pointer to the parent.
 * \param name    Name of the node.
 * \param data    The data inside the node.
 * \return        A pointer to the object.
 */
tnode *tnode_init(tnode *p, char *name, void *data);

/**
 * \brief Set the children of the node.
 *
 * \param t     The node to modify.
 * \param l     Left child.
 * \param r     Right child.
 */
void tnode_set_children(tnode *t, tnode *l, tnode *r);

/**
 * \brief Number of edges in the subtree.
 *
 * \param t    The subtree to analyze.
 * \return     The number of edges in the subtree.
 */
unsigned int tnode_numedges(tnode *t);

/**
 * \brief Number of leaves in the subtree.
 *
 * \param t    The subtree to analyze.
 * \return     The number of leaves in the subtree.
 */
unsigned int tnode_numleaves(tnode *t);

/**
 * \brief Number of nodes between this node and the root.
 *
 * \param t    The subtree to analyze.
 * \return     Number of nodes between this node and the root.
 */
unsigned int tnode_toroot(tnode *t);

/** Check if the subtree is really a strictly binary tree. */
int tnode_sbinary(tnode *t);

/**
 * \brief  Return 'true' if the node is a leaf.
 * 
 * \param t    The subtree to analyze.
 * \return     1 (TRUE) if the node is a leaf, 0 (FALSE) otherwise.
 */
int tnode_leaf(tnode *t);

/**
 * \brief  Return 'true' if the node is a root.
 *
 * \param t    The subtree to analyze.
 * \return     1 (TRUE) if the node is the root, 0 (FALSE) otherwise.
 */
int tnode_root(tnode *t);

/**
 * \brief  Return 'true' if the node is an internal node.
 *
 * \param t    The subtree to analyze.
 * \return     1 (TRUE) if the node is internal, 0 (FALSE) otherwise.
 */
int tnode_internal(tnode *t);

/**
 * \brief Return a the tree as a string in Newick format.
 *
 * \param t    The root of the tree used to build the string.
 * \return     A string in Newick format.
 */
char *tnode_simple_newick(tnode *t);

/**
 * \brief Recursively free the memory of the node.
 *
 * The tree node doesn't own the data at the end of the void pointer so the user
 * has to free this memory manually (or reimplement this function).
 * 
 * \param t    The root of the tree to free.
 */
void tnode_free(tnode *t);

#ifdef __cplusplus
}
#endif

#endif
