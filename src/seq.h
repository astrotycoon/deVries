/*! \file
 *
 * \brief Basic functions to analyze sequences.
 */ 

#ifndef SEQ_H_
#define SEQ_H_

#include <stdint.h>
#include <gsl/gsl_rng.h>
#include <gsl/gsl_randist.h>
#include "devries.h"

// For C++ compilers:
#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief Type of sequences.
 */
typedef enum
{
    Protein = 0,
    RNA = 1,
    DNA = 2,
    DNAorRNA = 3,
    Other = 4,
}
sequence_type;

/**
 * \brief A simple object to store FASTA sequences.
 */
typedef struct
{
    char *seq; /**< The sequence. */

    char *seq_info; /**< The 'info' on the sequence. */

    unsigned int length; /**< Length of the sequence. */

    sequence_type type; /**< Type of sequence. */
}
sequence;

/**
 * \brief A compressed sequence represented by a bitfield.
 */
typedef struct
{
    char *seq; /**< The sequence. */

    unsigned int length; /**< Length of the sequence. */

    unsigned int capacity; /**< Capacity of the object. */
}
cseq;

/**
 * \brief Extract the nth sequence from a file in fasta format.
 *
 * Extract a sequence and store it in a sequence object.
 * 
 * \param filename   Name of the input file.
 * \param n          Index of the sequence.
 * \param fasta      A pointer to an unitialized 'sequence' object.
 */
void read_fasta(const char *filename, unsigned int n, sequence *seq);

/**
 * \brief Extract the nth sequence from a file in EMBL format.
 *
 * Extract a sequence and store it in a sequence object.
 * 
 * \param filename   Name of the input file.
 * \param n          Index of the sequence.
 * \param fasta      A pointer to an unitialized 'sequence' object.
 */
void read_embl(const char *filename, unsigned int n, sequence *seq);

/**
 * \brief Extract the nth sequence from a file in genbank format.
 *
 * Extract a sequence and store it in a sequence object.
 * 
 * \param filename   Name of the input file.
 * \param n          Index of the sequence.
 * \param fasta      A pointer to an unitialized 'sequence' object.
 */
void read_genbank(const char *filename, unsigned int n, sequence *seq);

/**
 * \brief Return a random DNA nucleotide.
 * 
 * \param rng    A random number generator. 
 * \return       'A', 'T', 'C', or 'G'.
 */
char dna_random_nuc(gsl_rng *rng);

/**
 * \brief Return a random DNA nucleotide with custom probablity.
 *
 * Return a random nucleotide with given probabilities (with the probability for
 * C = 1 - prob(A + T + G)).
 * 
 * \param rng      A random number generator.
 * \param prob_a   Probability of 'A'.
 * \param prob_t   Probability of 'T'.
 * \param prob_g   Probability of 'G'.
 * \return         'A', 'T', 'C', or 'G'.
 */
char dna_random_nuc_prob(gsl_rng *rng, double prob_a, double prob_t, double prob_g);

/**
 * \brief Return a random DNA sequence.
 * 
 * \param rng        A random number generator.
 * \param seq_size   The length of the resulting sequence.
 * \return           A sequence of 'A', 'T', 'C', or 'G'.
 */
char *dna_random_nuc_seq(gsl_rng *rng, int seq_size);

/**
 * \brief Return a random RNA nucleotide.
 * 
 * \param rng    A random number generator. 
 * \return       'A', 'U', 'C', or 'G'.
 */
char rna_random_nuc(gsl_rng *rng);

/**
 * \brief Return a random RNA nucleotide with custom probablity.
 *
 * Return a random nucleotide with given probabilities (with the probability for
 * C = 1 - prob(A + U + G)).
 * 
 * \param rng      A random number generator.
 * \param prob_a   Probability of 'A'.
 * \param prob_t   Probability of 'U'.
 * \param prob_g   Probability of 'G'.
 * \return         'A', 'U', 'C', or 'G'.
 */
char rna_random_nuc_prob(gsl_rng *rng, double prob_a, double prob_u, double prob_g);

/**
 * \brief Return a random RNA sequence.
 * 
 * \param rng        A random number generator.
 * \param seq_size   The length of the resulting sequence.
 * \return           A sequence of 'A', 'U', 'C', or 'G'.
 */
char *rna_random_nuc_seq(gsl_rng *rng, int seq_size);

/**
 * \brief Count the occurences of a char in a string.
 * 
 * \param seq    A sequence. 
 * \return       The number of occurence.
 */
unsigned int seq_count(const char *seq);

/**
 * \brief Count the number of adenosine 'A' in the sequence.
 * 
 * \param seq    A sequence. 
 * \return       The number of adenosine 'A'.
 */
unsigned int a_count(const char *seq);

/**
 * \brief Count the number of thymidine 'T' in the sequence.
 * 
 * \param seq    A sequence. 
 * \return       The number of thymidine 'T'.
 */
unsigned int t_count(const char *seq);

/**
 * \brief Count the number of uracil 'U' in the sequence.
 * 
 * \param seq    A sequence. 
 * \return       The number of uracil 'U'.
 */
unsigned int u_count(const char *seq);

/**
 * \brief Count the number of guanine 'G' in the sequence.
 * 
 * \param seq    A sequence. 
 * \return       The number of guanine 'G'.
 */
unsigned int g_count(const char *seq);

/**
 * \brief Count the number of cytosine 'C' in the sequence.
 * 
 * \param seq    A sequence. 
 * \return       The number of cytosine 'C'.
 */
unsigned int c_count(const char *seq);

/**
 * \brief Count the number of cytosine 'C' and guanine 'G' in the sequence.
 * 
 * \param seq    A sequence. 
 * \return       The number of cytosine 'C' and guanine 'G'.
 */
unsigned int gc_count(const char *seq);

/** Return GC content (number of 'G' + number of 'C') / size. */
double gc_content(const char *seq);

/**
 * \brief Return TRUE if the sequence is only made of 'G', 'C', 'T' or 'A'.
 * 
 * \param dna_seq    A DNA sequence. 
 * \return           1 (TRUE) if the sequence is made of 'G', 'C', 'T' or 'A'.
 */
int dna_pure_seq(const char *dna_seq);

/**
 * \brief Return TRUE if the sequence is only made of 'G', 'C', 'U' or 'A'.
 * 
 * \param rna_seq    A RNA sequence. 
 * \return           1 (TRUE) if the sequence is made of 'G', 'C', 'U' or 'A'.
 */
int rna_pure_seq(const char *rna_seq);

/**
 * \brief Return a sequence with everything removed except 'G', 'C', 'T' and 'A'.
 * 
 * \param dna_seq    A DNA sequence. 
 * \return           A DNA sequence without the ambiguities.
 */
char *dna_rmv_amb(char *dna_seq);

/**
 * \brief Return a sequence with everything removed except 'G', 'C', 'U' and 'A'.
 * 
 * \param rna_seq    A RNA sequence. 
 * \return           A RNA sequence without the ambiguities.
 */
char *rna_rmv_amb(char *rna_seq);

/**
 * \brief Return the antisense strand of a DNA sequence.
 * 
 * \param dna_seq    A DNA sequence. 
 * \return           The antisense strand.
 */
char *dna_antisense(const char *dna_seq);

/**
 * \brief DNA -> RNA.
 *
 * Start reading when encountering a start codon. Both the DNA sequence and the
 * resulting RNA sequence are in the 5' -> 3' direction.
 * 
 * \param dna_seq    A DNA sequence. 
 * \return           RNA sequence resulting from transcription.
 */
char *transcription(const char *dna_seq);

/**
 * \brief Translate RNA to an amino acid sequence.
 *
 * \param dna_seq    A RNA sequence. 
 * \return           A sequence of amino acids.
 */
char *translation(const char *rna_seq);

/**
 * \brief Return true if the char is a standard DNA nucleotide.
 */
#define DNANUC(c) (c=='A'||c=='T'||c=='G'||c=='C')

/**
 * \brief Return true if the char is a standard RNA nucleotide.
 */
#define RNANUC(c) (c=='A'||c=='U'||c=='G'||c=='C')

#ifdef __cplusplus
}
#endif

#endif
