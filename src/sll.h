/*! \file
 *
 * \brief A simple generic singly linked list
 */ 

#ifndef SLL_H_
#define SLL_H_

/**
 * \brief The generic node of a singly linked list.
 */
typedef struct SLLNode_
{
    struct SLLNode_ *next; /**< Next node. */

    void *data; /**< Data in the node. */
}
SLLNode;

/**
 * \brief A generic singly linked list.
 */
typedef struct
{
    unsigned int length; /**< Number of elements in the list. */

    SLLNode *head; /**< First element of the list. */

    SLLNode *tail; /**< Last element of the list. */
}
SLL;

/**
 * \brief Initialize a singly linked list object.
 * 
 * \param sll  The object to initialize.
 */
void SLL_init(SLL *sll);

/**
 * \brief Add a node at the beginning of the list.
 *
 * \param sll    The singly linked list.
 * \param data   The data in the new node.
 */
void SLL_add_head(SLL *sll, void *data);

/**
 * \brief Add a node after the node supplied.
 * 
 * \param sll    The singly linked list.
 * \param node   The node just before the new node. Cannot be a NULL pointer.
 * \param data   The data in the new node.
 */
void SLL_add_after(SLL *sll, SLLNode *node, void *data);

/**
 * \brief Add a node at the end of the list.
 * 
 * \param sll    The singly linked list.
 * \param data   The data in the new node.
 */
void SLL_add_tail(SLL *sll, void *data);

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
int SLL_rm_next(SLL *sll, SLLNode *node);

/**
 * \brief Remove all nodes.
 * 
 * \param sll  The singly linked list.
 */
void SLL_rm_all(SLL *sll);

/**
 * \brief Remove all nodes satisfying a condition set by a function.
 *
 * You must supply a function that will take a SLLNode pointer and return true
 * (anything except 0) for the node to stay in the list and 0 to remove the node.
 * 
 * \param sll  The singly linked list.
 * \param foo  The condition to decide if a node can stay or has to be removed.
 * \return     The number of nodes removed.
 */
unsigned int SLL_rm(SLL *sll, int foo(SLLNode *node));

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
SLLNode *SLL_get(const SLL *sll, int n);

/**
 * \brief Generates an array from the data inside all the nodes.
 * 
 * \param sll  The singly linked list.
 * \return     A pointer to the array or void pointers.
 */
void **SLL_as_array(const SLL *sll);

/**
 * \brief Make sure the list has the same number of elements as 'length'.
 * 
 * \param sll  The singly linked list.
 * \return     0 if it doesn't match or the length of the list if it does.
 */
unsigned int SLL_check_length(const SLL *sll);

/**
 * \brief Free the memory of the list.
 *
 * Free the memory of the list but doesn't touch the void pointers.
 * 
 * \param sll  The singly linked list to free.
 */
void SLL_free(SLL *sll);

#endif
