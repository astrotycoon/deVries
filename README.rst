deVries
=======
Efficient data-structures to handle mutations and mutation profiles. The library
is written to be fast, straightforward, and simple to use. All functions are
documented and tests/examples are provided.

The library is named after Hugo de Vries (1848-1935), an important but often
forgotten scientist who rediscovered Mendel's law of genetics and coined the
term 'Mutation' (from the latin muto, 'to change').

Author
------
* Philippe Desjardins-Proulx <philippe.d.proulx@gmail.com>

Language
--------
C++-compatible ANSI C. See TESTS for a list of compiler tested.

Design
------
Mutations form the nodes of a tree. The tree is represented using a pointer to 
a singly linked list that contains the children. Only the initial sequence is
kept, but functions are provided to build a sequence from any node of the tree
(in this case, the function make a list of the mutations between the node and
the root and apply them to form a sequence).

For v.1.0, deVries should be able to read Fasta and SequenceML and print
mutation profiles and mutation trees in a XML format.

License
-------
MIT <http://www.opensource.org/licenses/mit-license.php>.

Dependencies
------------
* libxml2

On Linux Debian/Ubuntu you can install libxml2 with:

    sudo apt-get install libxml2 libxml2-dev

To compile a program with libxml2 you need to add, at the end of the command (yes it matters):

    $(xml2-config --libs) $(xml2-config --cflags)

For example a simple program using libxml2 could be compiled with:

    gcc -Wall -O3 -o main main.c $(xml2-config --libs) $(xml2-config --cflags)

Current to-do list
------------------
* Remove dependencies on the GSL library.
* Work & test singly linked list and tree.
* Design the mutation tree object.
* Input/Output format.
