#include <stdio.h>
#include <stdlib.h>
#include "devries.h"
#include "sll.h"

void sll_init()
{
    sll->length = 0;
    sll->head = NULL;
    sll->tail = NULL;
}

void sll_add_head(, void *data)
{
    sllnode *new_node = (sllnode*)malloc(sizeof(sllnode));
    new_node->data = data;
    new_node->next = sll->head;
    sll->head = new_node;
    
    if (sll->length == 0)
    {
        sll->tail = new_node;
    }
    ++(sll->length);
}

void sll_add_after(, sllnode *node, void *data)
{
    assert(node != NULL);
    sllnode *new_node = (sllnode*)malloc(sizeof(sllnode));
    new_node->data = data;
    new_node->next = node->next;
    node->next = new_node;
    
    if (new_node->next == NULL)
    {
        sll->tail = new_node;
    }
    ++(sll->length);
}

void sll_add_tail(, void *data)
{
    sllnode *new_node = (sllnode*)malloc(sizeof(sllnode));
    new_node->data = data;
    new_node->next = NULL;
    
    if (sll->length == 0)
    {
        sll->head = new_node;
        sll->tail = new_node;
    }
    else
    {
        sll->tail->next = new_node;
        sll->tail = new_node;
    }
    ++(sll->length);
}

int sll_rm_next(, sllnode *node)
{
    sllnode *old_node;

    if (sll->length == 0)
    {
        return false;
    }
    // Remove the head;
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

        // Update the 'tail' if necessary:
        if (node->next == NULL)
        {
            sll->tail = node;
        }
    }
    //////////////////////////////////////////
    // FREE THE MEMORY OF THE OLD NODE !!!  //
    //////////////////////////////////////////
    free(old_node);
    
    --(sll->length);
    return TRUE;
}

void sll_rm_all()
{
    while(sll_rm_next(sll, NULL));
}

unsigned int sll_rm(, int foo(sllnode *node))
{
    unsigned int removed = 0;

    sllnode *node = sll->head;

    // rmv first

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

sllnode *sll_get(, int n)
{
    if (n < 0 || n >= sll->length)
    {
        return NULL;
    }
    if (n == 0)
    {
        return sll->head;
    }
    if (n == sll->length - 1)
    {
        return sll->tail;
    }
    
    sllnode *node = sll->head;
    int i;
    for (i = 0; i < n; ++i)
    {
        node = node->next;
    }
    return node;
}

void **sll_as_array()
{
    assert(sll_check_length(sll));
    void **data = (void**)malloc(sll->length * sizeof(void*));

    int i = 0;
    sllnode *node = sll->head;
    for (; node != NULL; node = node->next)
    {
        data[i++] = node->data;
    }
    return data;
}

unsigned int sll_check_length(const )
{
    unsigned int length = 0;
    sllnode *node = sll->head;
    while (node != NULL)
    {
        ++length;
        node = node->next;
    }
    return (length == sll->length) ? length : 0;
}

void sll_free();
