#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include <stdbool.h>
#include <string.h>
#include "common.h"
#include "tnode.h"

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

M_INLINE void tnode_set_children(tnode *t, tnode *l, tnode *r)
{
    t->l = l;
    t->r = r;
}

#ifndef NDEBUG
M_INLINE unsigned int tnode_children(tnode *t)
{
    return t->children->length;
}
#endif

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

unsigned int tnode_nleaves(tnode *t)
{
    return 1 + tnode_nedges(t) / 2;
}

unsigned int tnode_toroot(tnode *t)
{
    return (t->p == NULL) ? 0 : 1 + tnode_toroot(t->p);
}

#ifndef NDEBUG
M_INLINE int tnode_leaf(tnode *t)
{
    return (t->children->length == 0);
}

M_INLINE int tnode_root(tnode *t)
{
    return (t->p == NULL);
}

M_INLINE int tnode_internal(tnode *t)
{
    return ((t->children->length > 0) && t->p != NULL);
}
#endif

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

void tnode_free(tnode *t)
{
    /*
    if (t->l != NULL)
    {
        tnode_free(t->l);
    }
    if (t->r != NULL)
    {
        tnode_free(t->r);
    }
    free(t->name);
    t->data = NULL;
    free(t);
    */
}
