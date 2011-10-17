#include <stdio.h>
#include <stdlib.h>
#include "common.h"
#include "sll.h"

void SLL_init(SLL *sll)
{
    sll->length = 0;
    sll->head = NULL;
    sll->tail = NULL;
}

void SLL_add_head(SLL *sll, void *data)
{
    SLLNode *new_node = (SLLNode*)malloc(sizeof(SLLNode));
    new_node->data = data;
    new_node->next = sll->head;
    sll->head = new_node;
    
    if (sll->length == 0)
    {
        sll->tail = new_node;
    }
    ++(sll->length);
}

void SLL_add_after(SLL *sll, SLLNode *node, void *data)
{
    assert(node != NULL);
    SLLNode *new_node = (SLLNode*)malloc(sizeof(SLLNode));
    new_node->data = data;
    new_node->next = node->next;
    node->next = new_node;
    
    if (new_node->next == NULL)
    {
        sll->tail = new_node;
    }
    ++(sll->length);
}

void SLL_add_tail(SLL *sll, void *data)
{
    SLLNode *new_node = (SLLNode*)malloc(sizeof(SLLNode));
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

int SLL_rm_next(SLL *sll, SLLNode *node)
{
    SLLNode *old_node;

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

void SLL_rm_all(SLL *sll)
{
    while(SLL_rm_next(sll, NULL));
}

unsigned int SLL_rm(SLL *sll, int foo(SLLNode *node))
{
    unsigned int removed = 0;

    SLLNode *node = sll->head;

    // rmv first

    while (node != NULL)
    {
        if (foo(node->next))
        {
            SLL_rm_next(sll, node);
        }
        else
        {
            node = node->next;
        }
    }
    
    return removed;
}

SLLNode *SLL_get(SLL *sll, int n)
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
    
    SLLNode *node = sll->head;
    int i;
    for (i = 0; i < n; ++i)
    {
        node = node->next;
    }
    return node;
}

void **SLL_as_array(SLL *sll)
{
    assert(SLL_check_length(sll));
    void **data = (void**)malloc(sll->length * sizeof(void*));

    int i = 0;
    SLLNode *node = sll->head;
    for (; node != NULL; node = node->next)
    {
        data[i++] = node->data;
    }
    return data;
}

unsigned int SLL_check_length(const SLL *sll)
{
    unsigned int length = 0;
    SLLNode *node = sll->head;
    while (node != NULL)
    {
        ++length;
        node = node->next;
    }
    return (length == sll->length) ? length : 0;
}

void SLL_free(SLL *sll);
