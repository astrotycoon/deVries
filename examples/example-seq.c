/**
 * This file contains tests and examples for the seq file
 *
 * Compiling
 * ---------
 * gcc -Wall -O3 -I../devries -o example-seq example-seq.c $(xml2-config --libs) $(xml2-config --cflags) -lm
 ******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include "seq.h"
#include "well1024.h"

int main()
{
    well1024 rng;
    well1024_init(&rng, 42);
    
    char *seq = dna_random_nuc_seq(&rng, 1000);

    fprintf(stdout, "seq: %s\n", seq);

    return EXIT_SUCCESS;
}
