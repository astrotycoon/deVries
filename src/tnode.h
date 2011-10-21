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

    unsigned int n; /**< Number of children. */
    
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
tnode *tnode_init(tnode *p, char *name, void *data)
{
    tnode *t = (tnode*)malloc(sizeof(tnode));

    t->name = name;
    t->p = p;
    t->l = NULL;
    t->r = NULL;
    t->data = NULL;

    return t;
}

/**
 * \brief Add a children to the node.
 *
 * \param t       The node to modify.
 * \param child   The new kid.
 */
void tnode_add_children(tnode *t, tnode *child)
{
    ++(t->n);
    sll_add_tail((t)->children,(void*)(child))
}

/**
 * \brief Number of edges in the subtree.
 *
 * \param t    The subtree to analyze.
 * \return     The number of edges in the subtree.
 */
unsigned int tnode_nedges(tnode *t)
{
    unsigned int nedges = 0;
    sllnode *n = t->children->head;
    for (; n != NULL; n = n->next)
    {
        nedges += tnode_nedges((tnode*)(n->data));
    }
    return nedges;
}

/**
 * \brief Number of leaves in the subtree.
 *
 * \param t    The subtree to analyze.
 * \return     The number of leaves in the subtree.
 */
unsigned int tnode_nleaves(tnode *t)
{
    return 1 + tnode_nedges(t) / 2;
}

/**
 * \brief Number of nodes between this node and the root.
 *
 * \param t    The subtree to analyze.
 * \return     Number of nodes between this node and the root.
 */
unsigned int tnode_toroot(tnode *t)
{
    return (t->p == NULL) ? 0 : 1 + tnode_toroot(t->p);
}

#ifndef NDEBUG
/**
 * \brief Return 'true' if the node is a leaf.
 * 
 * \param t    The subtree to analyze.
 * \return     1 (TRUE) if the node is a leaf, 0 (FALSE) otherwise.
 */
int tnode_leaf(tnode *t)
{
    return (t->n == 0);
}

/**
 * \brief Return 'true' if the node is a root.
 *
 * \param t    The subtree to analyze.
 * \return     1 (TRUE) if the node is the root, 0 (FALSE) otherwise.
 */
int tnode_root(tnode *t)
{
    return (t->p == NULL);
}

/**
 * \brief Return 'true' if the node is an internal node.
 *
 * \param t    The subtree to analyze.
 * \return     1 (TRUE) if the node is internal, 0 (FALSE) otherwise.
 */
int tnode_internal(tnode *t)
{
    return ((t->n > 0) && t->p != NULL);
}
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
char *tnode_newick(tnode *t)
{
    char *str;
    /*
    if (tnode_leaf(t))
    {
        str = (char*)malloc(strlen(t->name) + 2);
        if (t->p == NULL)
        {
            sprintf(str, "%s;", t->name);
        }
        else
        {
            sprintf(str, "%s", t->name);
        }
    }
    else
    {
        char *lnewick = tnode_simple_newick(t->l);
        char *rnewick = tnode_simple_newick(t->r);
        const int lrlength = strlen(lnewick) + strlen(rnewick);
        str = (char*)malloc(lrlength + strlen(t->name) + 6);
        if (t->p == NULL)
        {
            sprintf(str, "(%s,%s):%s;", lnewick, rnewick, t->name);
        }
        else
        {
            sprintf(str, "(%s,%s):%s", lnewick, rnewick, t->name);
        }
        free(lnewick);
        free(rnewick);
    }
    */
    return str;
}

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
