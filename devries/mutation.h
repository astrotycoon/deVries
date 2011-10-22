/*! \file
 *
 * \brief An object to description mutations.
 */ 

#ifndef MUTATION_H_
#define MUTATION_H_

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <libxml/parser.h>
#include "devries.h"
#include "tnode.h"
#include "sll.h"
#include "seq.h"

/* For C++ compilers: */
#ifdef __cplusplus
extern "C"
{
#endif

/**
 * \brief Tyes of mutations.
 */
typedef enum
{
    Point = 0,
    Insertions = 1,
    Deletions = 2,
}
mut_type;

/**
 * \brief Used by the 'mutation' object to store info on different mutations.
 */
typedef union
{
    char newc; /**< For point mutations: the new nucleotide. */

    char *insert; /**< For insertions: the string to insert. */

    unsigned int ndels; /**< For dels: number of elements to delete. */
}
mut_info;

/**
 * \brief A mutation.
 */
typedef struct
{
    mut_type type; /**< Type of mutation. */

    unsigned int pos; /**< Position of the mutation. */

    mut_info mut; /**< Details on the mutation. */
}
mutation;

/**
 * \brief A tree of mutations.
 */
typedef struct
{
    char *seq; /**< Initial sequence. */

    tnode *root; /**< Root of the mutation tree. */
}
mutation_tree;

/**
 * \brief Build the complete sequence for a node of the tree.
 *
 * This function will apply the mutations from this node to the root of the
 * mutation tree to generate a sequence.
 *
 * \param node    Node of the mutation tree.
 * \return        Sequence.
 */
char *get_sequence(tnode *node);

/**
 * \brief Create a singly linked list of all mutations in and under the node.
 *
 * \param node    Node of the mutation tree.
 * \return        Singly-linked list with all mutations.
 */
sll *list_mutations(tnode *node);

/**
 * \brief Apply a mutation to a sequence.
 *
 * Apply a mutation on a sequence. If necessary, memory will be reallocated.
 * This function can deal with any type of mutation but more specialized
 * function are also available.
 *
 * \param seq     A pointer to the sequence.
 * \param mut     Mutation object.
 * \return        A pointer to the sequence (will only change if memory has been reallocated).
 */
char *apply_mut(char **seq, mutation *m);

/**
 * \brief Apply a point mutation to a sequence.
 *
 * \param seq     A pointer to the sequence.
 * \param mut     Mutation object.
 * \return        A pointer to the sequence (will only change if memory has been reallocated).
 */
void apply_point(char *seq, mutation *m);
{
    seq[mut->pos] = mut->newc;
}

/**
 * \brief Apply an insert mutation.
 *
 * This function will reallocate memory to accomodate the new elements.
 *
 * \param seq     A pointer to the sequence.
 * \param mut     Mutation object.
 * \return        A pointer to the new sequence.
 */
char *apply_insert(char **seq, mutation *m);

/**
 * \brief Apply a del mutation.
 *
 * This function will *not* reallocate memory (see apply_del_realloc). 
 * 
 * \param seq     A pointer to the sequence.
 * \param mut     Mutation object.
 * \return        A pointer to the sequence.
 */
char *apply_del(char **seq, mutation *m);

/**
 * \brief Apply a del mutation.
 *
 * Reallocate memory. Save space but is less efficient than apply_del.
 * 
 * \param seq     A pointer to the sequence.
 * \param mut     Mutation object.
 * \return        A pointer to the sequence.
 */
char *apply_del_realloc(char **seq, mutation *m);

/**
 * \brief Return a mutated sequence without modifying the original.
 * 
 * \param seq     A pointer to the sequence.
 * \param mut     Mutation object.
 * \return        A new sequence.
 */
char* get_mut(const char *seq, mutation *m)
{
    unsigned int length0 = len(seq);
    unsigned int length1 = length0;
    if (m->type == Insertions)
    {
        length1 += strlen(m->mut.insert);
    }
    else if (m->type = Deletions)
    {
        length1 -= m->mut.ndels;
    }
    char *seq1 = (char*)malloc(length1 + 1);

    if (m->type == Point)
    {
        memcpy(seq1, seq, length);
        seq1[mut->pos] = mut->newc;
    }
    if (m->type == Inserts)
    {
        const unsigned int insert_length = strlen(m->mut.insert);
        memcpy(seq1, seq, m->pos);
        memcpy(seq1 + m->pos, m->mut.insert, insert_length);
        memcpy(seq1 + m->pos + insert_length, seq, length - m->pos);
    }
    else /* Delete. */
    {
        memcpy(seq1, seq, m->pos);
        memcpy(seq1 + m->pos, seq, length - m->pos);
    }
    seq1[length1] = '\0';
    return seq1;
}

#ifdef __cplusplus
}
#endif

#endif
