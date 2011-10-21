/**
 * This file contains tests and examples for the singly linked list.
 *
 * Compiling
 * ---------
 * gcc -Wall -O3 -I../devries -o example-sll example-sll.c $(xml2-config --libs) $(xml2-config --cflags)
 ******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include "sll.h"

int main()
{
    /* Create a list object: */
    sll *list;

    /* Initialize the object: */
    sll_init(list);
    
    char *str = (char*)malloc(50);
    sprintf(str, "Odin");

    return EXIT_SUCCESS; // Yeppie !
}
