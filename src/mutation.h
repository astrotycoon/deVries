/*! \file
 *
 * \brief An object to description mutations.
 *
 */ 

#ifndef tnode_H_
#define tnode_H_

#include <stdbool.h>
#include "common.h"

// For C++ compilers:
#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief A strictly binary tree.
 */
typedef enum
{
    /* */
}
mutation_type;

typedef struct
{
    mutation_type type; /**< Type of mutation. */

    /* */
}
mutation;

typedef struct
{
    tnode *root; /**< Root of the mutation tree. */
}
mutation_tree;

char *

#ifdef __cplusplus
}
#endif

#endif
