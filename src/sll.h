/*! \file
 *
 * \brief A simple generic singly linked list.
 */ 

#ifndef sll_H_
#define sll_H_

/**
 * \brief The generic node of a singly linked list.
 */
typedef struct sllnode_
{
    struct sllnode_ *next; /**< Next node. */

    void *data; /**< Data in the node. */
}
sllnode;

/**
 * \brief A generic singly linked list.
 */
typedef struct
{
    unsigned int length; /**< Number of elements in the list. */

    sllnode *head; /**< First element of the list. */

    sllnode *tail; /**< Last element of the list. */
}
sll;

/**
 * \brief Initialize a singly linked list object.
 * 
 * \param l  The object to initialize.
 */
void sll_init(sll *l);

/**
 * \brief Add a node at the beginning of the list.
 *
 * \param sll    The singly linked list.
 * \param data   The data in the new node.
 */
void sll_add_head(sll *l, void *data);

/**
 * \brief Add a node after the node supplied.
 * 
 * \param sll    The singly linked list.
 * \param node   The node just before the new node. Cannot be a NULL pointer.
 * \param data   The data in the new node.
 */
void sll_add_after(sll *l, sllnode *node, void *data);

/**
 * \brief Add a node at the end of the list.
 * 
 * \param sll    The singly linked list.
 * \param data   The data in the new node.
 */
void sll_add_tail(sll *l, void *data);

/**
 * \brief Remove the node next to the provided node.
 *
 * Provide a list and a node and the function will remove the next node. If
 * node == NULL, the function will remove the head of the list.
 * 
 * \param sll    The singly linked list.
 * \param node   The node before the node to be removed.
 * \return       1 (TRUE) is a node has been removed.
 */
int sll_rm_next(sll *l, sllnode *node);

/**
 * \brief Remove all nodes.
 * 
 * \param sll  The singly linked list.
 */
void sll_rm_all(sll *l);

/**
 * \brief Remove all nodes satisfying a condition set by a function.
 *
 * You must supply a function that will take a sllnode pointer and return true
 * (anything except 0) for the node to stay in the list and 0 to remove the node.
 * 
 * \param sll  The singly linked list.
 * \param foo  The condition to decide if a node can stay or has to be removed.
 * \return     The number of nodes removed.
 */
unsigned int sll_rm(sll *l, int foo(sllnode *node));

/**
 * \brief Return a pointer to the nth node in the list.
 *
 * This function will either return a pointer to the nth node in the list
 * or a NULL pointer if n is out of range.
 * 
 * \param sll  The singly linked list.
 * \param n    The index of the node to return
 * \return     A pointer to the nth node.
 */
sllnode *sll_get(const sll *l, int n);

/**
 * \brief Generates an array from the data inside all the nodes.
 * 
 * \param sll  The singly linked list.
 * \return     A pointer to the array or void pointers.
 */
void **sll_as_array(const sll *l);

/**
 * \brief Make sure the list has the same number of elements as 'length'.
 * 
 * \param sll  The singly linked list.
 * \return     0 if it doesn't match or the length of the list if it does.
 */
unsigned int sll_check_length(const sll *l);

/**
 * \brief Free the memory of the list.
 *
 * Free the memory of the list but doesn't touch the void pointers.
 * 
 * \param sll  The singly linked list to free.
 */
void sll_free(sll *l);

#endif
