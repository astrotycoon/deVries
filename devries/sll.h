/*! \file
 *
 * \brief A simple generic singly linked list.
 */ 

#ifndef SLL_H_
#define SLL_H_

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "devries.h"

/* For C++ compilers: */
#ifdef __cplusplus
extern "C"
{
#endif

/**
 * \brief The node of a singly linked list.
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
    sllnode *head; /**< First element of the list. */

    sllnode *tail; /**< Last element of the list. */
}
sll;

/**
 * \brief Initialize a singly linked list object.
 * 
 * \param l  The object to initialize.
 */
void sll_init(sll *l)
{
    sll->head = NULL;
    sll->tail = NULL;
}

/**
 * \brief Return a pointer to the ith node in the list.
 *
 * This function will either return a pointer to the nth node in the list
 * or a NULL pointer if i is out of range.
 * 
 * \param l    The singly linked list.
 * \param i    The index of the node to return
 * \return     A pointer to the ith node.
 */
sllnode *sll_get(sll *l, unsigned int i)
{
    if (i < 0)
    {
        return NULL;
    }
    if (i == 0)
    {
        return sll->head;
    }    
    sllnode *node = sll->head;
    unsigned int j = 0;
    for (; j < i; ++j)
    {
        node = node->next;
    }
    return node;
}

/**
 * \brief Add a node at the beginning of the list.
 *
 * \param l      The singly linked list.
 * \param data   The data in the new node.
 */
void sll_add_head(sll *l, void *data)
{
    sllnode *new_node = (sllnode*)malloc(sizeof(sllnode));
    new_node->data = data;
    new_node->next = sll->head;
    sll->head = new_node;
    
    if (sll->tail == NULL)
    {
        sll->tail = new_node;
    }
}

/**
 * \brief Add a node after the node supplied.
 * 
 * \param l      The singly linked list.
 * \param node   The node just before the new node. Cannot be a NULL pointer.
 * \param data   The data in the new node.
 */
void sll_add_after(sll *l, sllnode *node, void *data)
{
    if (node == NULL)
    {
        sll_add_head(l, data);
    }
    sllnode *new_node = (sllnode*)malloc(sizeof(sllnode));
    new_node->data = data;
    new_node->next = node->next;
    node->next = new_node;
    
    if (new_node->next == NULL)
    {
        sll->tail = new_node;
    }
}

/**
 * \brief Add a node after the ith node.
 * 
 * \param l      The singly linked list.
 * \param i      ID of the node before the new node.
 * \param data   The data in the new node.
 */
void sll_add_after_n(sll *l, unsigned int i, void *data)
{
    sll_add_after(l, sll_get(l, i), data);
}

/**
 * \brief Add a node at the end of the list.
 * 
 * \param l      The singly linked list.
 * \param data   The data in the new node.
 */
void sll_add_tail(sll *l, void *data)
{
    sllnode *new_node = (sllnode*)malloc(sizeof(sllnode));
    new_node->data = data;
    new_node->next = NULL;
    
    if (sll->head == NULL)
    {
        sll->head = new_node;
    }
    else
    {
        sll->tail->next = new_node;
    }
    sll->tail = new_node;
}

/**
 * \brief Remove the node next to the provided node.
 *
 * Remove the node next to the supplied node. If node == NULL, the function will
 * remove the head of the list.
 * 
 * \param l      The singly linked list.
 * \param node   The node before the node to be removed.
 * \return       1 (TRUE) is a node has been removed.
 */
int sll_rm_next(sll *l, sllnode *node)
{
    sllnode *old_node;

    if (sll->head == NULL)
    {
        return FALSE;
    }
    if (node == NULL)
    {
        old_node = sll->head;
        sll->head = sll->head->next;
    }
    else
    {
        if (node->next == NULL)
        {
            return FALSE;
        }
        old_node = node->next;
        node->next = node->next->next;

        /* Update the 'tail' if necessary: */
        if (node->next == NULL)
        {
            sll->tail = node;
        }
    }
    free(old_node);

    return TRUE;
}

/**
 * \brief Remove all nodes.
 * 
 * \param l    The singly linked list.
 */
void sll_rm_all(sll *l)
{
    while(sll_rm_next(sll, NULL));
}

/**
 * \brief Remove all nodes satisfying a condition set by a function.
 *
 * You must supply a function that will take a sllnode pointer and return true
 * (anything except 0) for the node to stay in the list and 0 to remove the node.
 * 
 * \param l    The singly linked list.
 * \param foo  The condition to decide if a node can stay or has to be removed.
 * \return     The number of nodes removed.
 */
unsigned int sll_rm(sll *l, int foo(sllnode *node))
{
    unsigned int removed = 0;

    sllnode *node = sll->head;

    /* rmv first */

    while (node != NULL)
    {
        if (foo(node->next))
        {
            sll_rm_next(sll, node);
        }
        else
        {
            node = node->next;
        }
    }
    return removed;
}

/**
 * \brief Generates an array from the data inside all the nodes.
 * 
 * \param l    The singly linked list.
 * \return     A pointer to the array or void pointers.
 */
void **sll_as_array(sll *l)
{
    void **data = (void**)malloc(sll_length(l) * sizeof(void*));

    int i = 0;
    sllnode *node = sll->head;
    for (; node != NULL; node = node->next)
    {
        data[i++] = node->data;
    }
    return data;
}

/**
 * \brief Return the length.
 * 
 * \param l    The singly linked list.
 * \return     Number of nodes in the list.
 */
unsigned int sll_length(const sll *l)
{
    unsigned int length = 0;
    sllnode *node = sll->head;
    while (node != NULL)
    {
        ++length;
        node = node->next;
    }
    return length;
}

/**
 * \brief Free the memory of the list.
 *
 * Free the memory of the list but doesn't touch the void pointers.
 * 
 * \param l    The singly linked list to free.
 */
void sll_free(sll *l);

#endif
