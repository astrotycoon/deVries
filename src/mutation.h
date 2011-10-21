/*! \file
 *
 * \brief An object to description mutations.
 *
 */ 

#ifndef MUTATION_H_
#define MUTATION_H_

#include <stdbool.h>
#include "common.h"
#include "tnode.h"
#include "sll.h"

// For C++ compilers:
#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief Tyes of mutations.
 *
 * For now, the library only support point mutation.
 */
typedef enum
{
    Point = 0,
    Frameshift = 1,
    Inversion = 2,
    Duplication = 3,
}
mutation_type;

/**
 * \brief A mutation.
 *
 * For now, the library only support point mutation.
 */
typedef struct
{
    mutation_type type; /**< Type of mutation. */

    /* */
}
mutation;

/** */
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
char *apply_mut(char **seq, mutation *mut);

/**
 * \brief Apply a point mutation to a sequence.
 *
 * \param seq     A pointer to the sequence.
 * \param mut     Mutation object.
 * \return        A pointer to the sequence (will only change if memory has been reallocated).
 */
void apply_point(char *seq, mutation *mut)
{
    seq[mut->pos] = mut->c; // example
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
char *apply_insert(char **seq, mutation *mut);

/**
 * \brief Apply a del mutation.
 *
 * This function will *not* reallocate memory (see apply_del_realloc). 
 * 
 * \param seq     A pointer to the sequence.
 * \param mut     Mutation object.
 * \return        A pointer to the sequence.
 */
char *apply_del(char **seq, mutation *mut);

/**
 * \brief Apply a del mutation.
 *
 * Reallocate memory. Save space but is less efficient than apply_del.
 * 
 * \param seq     A pointer to the sequence.
 * \param mut     Mutation object.
 * \return        A pointer to the sequence.
 */
char *apply_del_realloc(char **seq, mutation *mut);

#ifdef __cplusplus
}
#endif

#endif
