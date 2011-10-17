/*! \file
 *
 * \brief A generic strictly binary tree.
 *
 */ 

#ifndef TNODE_H_
#define TNODE_H_

#include "devries.h"
#include "sll.h"

// For C++ compilers:
#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief A generic tree.
 */
typedef struct tnode_
{
    char *name; /**< Name of the node. */

    struct tnode_ *p; /**< Pointer to the parent. */
    
    sll *children; /**< Singly linked list of children. */

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
 * \brief Add a children to the node.
 *
 * \param t       The node to modify.
 * \param child   The new kid.
 */
void tnode_add_children(tnode *t, tnode *child);

#ifndef NDEBUG
/**
 * \brief Get the number of children in the node.
 *
 * \param t     The tnode object.
 * \return      Number of children in the node.
 */
unsigned int tnode_children(tnode *t);
#else
#define tnode_children(t)    ((t)->children->length)
#endif

/**
 * \brief Number of edges in the subtree.
 *
 * \param t    The subtree to analyze.
 * \return     The number of edges in the subtree.
 */
unsigned int tnode_nedges(tnode *t);

/**
 * \brief Number of leaves in the subtree.
 *
 * \param t    The subtree to analyze.
 * \return     The number of leaves in the subtree.
 */
unsigned int tnode_nleaves(tnode *t);

/**
 * \brief Number of nodes between this node and the root.
 *
 * \param t    The subtree to analyze.
 * \return     Number of nodes between this node and the root.
 */
unsigned int tnode_toroot(tnode *t);

#ifndef NDEBUG
/**
 * \brief Return 'true' if the node is a leaf.
 * 
 * \param t    The subtree to analyze.
 * \return     1 (TRUE) if the node is a leaf, 0 (FALSE) otherwise.
 */
int tnode_leaf(tnode *t);

/**
 * \brief Return 'true' if the node is a root.
 *
 * \param t    The subtree to analyze.
 * \return     1 (TRUE) if the node is the root, 0 (FALSE) otherwise.
 */
int tnode_root(tnode *t);

/**
 * \brief Return 'true' if the node is an internal node.
 *
 * \param t    The subtree to analyze.
 * \return     1 (TRUE) if the node is internal, 0 (FALSE) otherwise.
 */
int tnode_internal(tnode *t);
#else
#ifndef NDEBUG
#define tnode_leaf(t)       ((t)->children->length==0)
#define tnode_root(t)       ((t)->p==NULL)
#define tnode_internal(t)   (((t)->children->length>0)&&t->p!=NULL)
#endif

/**
 * \brief Return a the tree as a string in Newick format.
 *
 * \param t    The root of the tree used to build the string.
 * \return     A string in Newick format.
 */
char *tnode_newick(tnode *t);

/**
 * \brief Recursively free the memory of the node.
 *
 * The tree node doesn't own the data at the end of the void pointer so the user
 * has to free this memory manually (or reimplement this function).
 * 
 * \param t    The root of the tree to free.
 */
void tnode_free(tnode *t); /* Add a void function for genericity. */

#ifdef __cplusplus
}
#endif

#endif
