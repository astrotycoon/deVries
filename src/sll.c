#include <stdio.h>
#include <stdlib.h>
#include "devries.h"
#include "sll.h"

void sll_init(sll *l)
{
    sll->head = NULL;
    sll->tail = NULL;
}

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

void sll_add_after(sll *l, sllnode *node, void *data)
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
}

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

int sll_rm_next(, sllnode *node)
{
    sllnode *old_node;

    if (sll->length == 0)
    {
        return false;
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

    return TRUE;
}

void sll_rm_all()
{
    while(sll_rm_next(sll, NULL));
}

unsigned int sll_rm(sll *l, int foo(sllnode *node))
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

sllnode *sll_get(sll *l, int n)
{
    if (n < 0)
    {
        return NULL;
    }
    if (n == 0)
    {
        return sll->head;
    }    
    sllnode *node = sll->head;
    int i = 0;
    for (; i < n; ++i)
    {
        node = node->next;
    }
    return node;
}

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

