/*! \file
 *
 * \brief Basic functions to analyze sequences.
 */ 

#ifndef SEQ_H_
#define SEQ_H_

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdint.h>
#include <string.h>
#include <libxml/parser.h>
#include "devries.h"
#include "gcode.h"

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
void read_fasta(const char *filename, unsigned int n, sequence *seq)
{
    FILE* input = fopen(filename, "r");

    fseek(input, 0, SEEK_END); // seek to end of file
    const long file_size = ftell(input); // get the size of the file
    fseek(input, 0, SEEK_SET); // seek back to beginning of file

    // Read the entire file
    char *complete_file = (char*)malloc(file_size);
    size_t fread_size = fread((void*)complete_file, sizeof(char), file_size, input);
    const unsigned int complete_file_size = strlen(complete_file);

    unsigned int i = 0;
    while (i < complete_file_size)
    {
        if (complete_file[i] == '>')
        {
            const int i0 = i;
            while (complete_file[i] != '\n')
            {
                ++i;
            }
            fasta->seq_info = (char*)malloc(i - i0 + 1);
            strncpy(fasta->seq_info, complete_file + i0, i - i0);

            fasta->seq = (char*)malloc(complete_file_size);
            int j = 0;

            //printf("%d\n", i);

            while (i < complete_file_size)
            {
                if (complete_file[i] != '\n' && complete_file[i] != '\r')
                {
                    fasta->seq[j++] = complete_file[i];
                }
                ++i;
            }
            fasta->seq[j] = '\0';
            fasta->length = strlen(fasta->seq);
            break;
        }
    }
    fasta->type = get_type(fasta->seq);
    fclose(fp);
}

/**
 * \brief Extract the nth sequence from a file in sequenceml format.
 *
 * Extract a sequence and store it in a sequence object.
 * 
 * \param filename   Name of the input file.
 * \param n          Index of the sequence.
 * \param fasta      A pointer to an unitialized 'sequence' object.
 */
void read_sequenceml(const char *filename, unsigned int n, sequence *seq)
{
    xmlDoc *document;
    xmlNode *root, *first_child, *node;
    char *filename;

    if (argc < 2)
    {
        fprintf(stderr, "Usage: %s filename.xml\n", argv[0]);
        return EXIT_FAILURE;
    }
    filename = argv[1];

    document = xmlReadFile(filename, NULL, 0);
    root = xmlDocGetRootElement(document);
    fprintf(stdout, "Root is <%s> (%i)\n", root->name, root->type);
    first_child = root->children;

    for (node = first_child; node; node = node->next)
    {
        fprintf(stdout, "\t Child is <%s> (%i)\n", node->name, node->type);
    }
    fprintf(stdout, "...\n");
}

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
char dna_random_nuc(gsl_rng *rng)
{
    return "ATGC"[(int)(gsl_rng_uniform(rng) * 4)];
}

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
char dna_random_nuc_prob(gsl_rng *rng, double prob_a, double prob_t, double prob_g)
{
    const double r = gsl_rng_uniform(rng);
    if (r < prob_a)
    {
        return 'A';
    }
    else if (r < prob_a + prob_t)
    {
        return 'T';
    }
    else if (r < prob_a + prob_t + prob_g)
    {
        return 'G';
    }
    else
    {
        return 'C';
    }
}

/**
 * \brief Return a random DNA sequence.
 * 
 * \param rng        A random number generator.
 * \param seq_size   The length of the resulting sequence.
 * \return           A sequence of 'A', 'T', 'C', or 'G'.
 */
char *dna_random_nuc_seq(gsl_rng *rng, int seq_size)
{
    assert(seq_size > 0);
    char *dna_seq = (char*)malloc(seq_size + 1);
    char nuc[4 + 1] = "ATGC";
    
    for (int i = 0; i < seq_size; ++i)
    {
        dna_seq[i] = nuc[(int)(gsl_rng_uniform(rng) * 4)];
    }
    dna_seq[seq_size] = '\0';
    return dna_seq;
}

/**
 * \brief Return a random RNA nucleotide.
 * 
 * \param rng    A random number generator. 
 * \return       'A', 'U', 'C', or 'G'.
 */
char rna_random_nuc(gsl_rng *rng)
{
    return "AUGC"[(int)(gsl_rng_uniform(rng) * 4)];
}

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
char rna_random_nuc_prob(gsl_rng *rng, double prob_a, double prob_u, double prob_g)
{
    const double r = gsl_rng_uniform(rng);
    if (r < prob_a)
    {
        return 'A';
    }
    else if (r < prob_a + prob_u)
    {
        return 'U';
    }
    else if (r < prob_a + prob_u + prob_g)
    {
        return 'G';
    }
    else
    {
        return 'C';
    }
}

/**
 * \brief Return a random RNA sequence.
 * 
 * \param rng        A random number generator.
 * \param seq_size   The length of the resulting sequence.
 * \return           A sequence of 'A', 'U', 'C', or 'G'.
 */
char* rna_random_nuc_seq(gsl_rng *rng, int seq_size)
{
    assert(seq_size > 0);
    char *rna_seq = (char*)malloc(seq_size + 1);
    char nuc[4 + 1] = "AUGC";
    
    for (int i = 0; i < seq_size; ++i)
    {
        rna_seq[i] = nuc[(int)(gsl_rng_uniform(rng) * 4)];
    }
    rna_seq[seq_size] = '\0';
    return rna_seq;
}

/**
 * \brief Count the occurences of a char in a string.
 * 
 * \param seq    A sequence. 
 * \return       The number of occurence.
 */
unsigned int seq_count(const char *seq, const char c)
{
    int i = -1;
    unsigned int count = 0;
    while (seq[++i] != '\0')
    {
        count += (seq[i] == c);
    }
    return count;
}

/**
 * \brief Count the number of adenosine 'A' in the sequence.
 * 
 * \param seq    A sequence. 
 * \return       The number of adenosine 'A'.
 */
unsigned int a_count(const char *seq)
{
    return seq_count(seq, 'A');
}

/**
 * \brief Count the number of thymidine 'T' in the sequence.
 * 
 * \param seq    A sequence. 
 * \return       The number of thymidine 'T'.
 */
unsigned int t_count(const char *seq)
{
    return seq_count(seq, 'T');
}

/**
 * \brief Count the number of uracil 'U' in the sequence.
 * 
 * \param seq    A sequence. 
 * \return       The number of uracil 'U'.
 */
unsigned int u_count(const char *seq)
{
    return seq_count(seq, 'U');
}

/**
 * \brief Count the number of guanine 'G' in the sequence.
 * 
 * \param seq    A sequence. 
 * \return       The number of guanine 'G'.
 */
unsigned int g_count(const char *seq)
{
    return seq_count(seq, 'G');
}

/**
 * \brief Count the number of cytosine 'C' in the sequence.
 * 
 * \param seq    A sequence. 
 * \return       The number of cytosine 'C'.
 */
unsigned int c_count(const char *seq)
{
    return seq_count(seq, 'C');
}

/**
 * \brief Count the number of cytosine 'C' and guanine 'G' in the sequence.
 * 
 * \param seq    A sequence. 
 * \return       The number of cytosine 'C' and guanine 'G'.
 */
unsigned int gc_count(const char *seq)
{
    int i = -1;
    unsigned int count = 0;
    while (seq[++i] != '\0')
    {
        count += (seq[i] == 'G' || seq[i] == 'C');
    }
    return count;
}

/**
 * \brief Return GC content (number of 'G' + number of 'C') / size.
 * 
 * \param seq    A sequence. 
 * \return       The proportion of cytosine 'C' and guanine 'G'.
 */
double gc_content(const char *seq)
{
    int i = -1;
    unsigned int count = 0;
    while (seq[++i] != '\0')
    {
        count += (seq[i] == 'G' || seq[i] == 'C');
    }
    return (double)count / i;
}

/**
 * \brief Return TRUE if the sequence is only made of 'G', 'C', 'T' or 'A'.
 * 
 * \param dna_seq    A DNA sequence. 
 * \return           1 (TRUE) if the sequence is made of 'G', 'C', 'T' or 'A'.
 */
int dna_pure_seq(const char *dna_seq)
{
    int i = 0;
    while (dna_seq[i] != '\0')
    {
        if (DNANUC(dna_seq[i]))
        {
            ++i;
        }
        else
        {
            return FALSE;
        }
    }
    return TRUE;
}

/**
 * \brief Return TRUE if the sequence is only made of 'G', 'C', 'U' or 'A'.
 * 
 * \param rna_seq    A RNA sequence. 
 * \return           1 (TRUE) if the sequence is made of 'G', 'C', 'U' or 'A'.
 */
int rna_pure_seq(const char *rna_seq)
{
    int i = 0;
    while (rna_seq[i] != '\0')
    {
        if (RNANUC(rna_seq[i]))
        {
            ++i;
        }
        else
        {
            return FALSE;
        }
    }
    return TRUE;
}

/**
 * \brief Return a sequence with everything removed except 'G', 'C', 'T' and 'A'.
 * 
 * \param dna_seq    A DNA sequence. 
 * \return           A DNA sequence without the ambiguities.
 */
char *dna_rmv_amb(char *dna_seq)
{
    char *new_dna_seq = (char*)malloc(strlen(dna_seq));

    int count = 0;
    for (int i = 0; dna_seq[i] != '\0'; i++)
    {
        if (DNANUC(dna_seq[i]))
        {
            new_dna_seq[count++] = dna_seq[i];
        }
    }
    new_dna_seq = (char*)realloc((void*)new_dna_seq, count + 1);
    new_dna_seq[count] = '\0';

    return new_dna_seq;
}

/**
 * \brief Return a sequence with everything removed except 'G', 'C', 'U' and 'A'.
 * 
 * \param rna_seq    A RNA sequence. 
 * \return           A RNA sequence without the ambiguities.
 */
char *rna_rmv_amb(char *rna_seq)
{
    char *new_rna_seq = (char*)malloc(strlen(rna_seq));

    int count = 0;
    for (int i = 0; rna_seq[i] != '\0'; i++)
    {
        if (RNANUC(rna_seq[i]))
        {
            new_rna_seq[count++] = rna_seq[i];
        }
    }
    new_rna_seq = (char*)realloc((void*)new_rna_seq, count + 1);

    new_rna_seq[count] = '\0';
    return new_rna_seq;
}

/**
 * \brief Return the antisense strand of a DNA sequence.
 * 
 * \param dna_seq    A DNA sequence. 
 * \return           The antisense strand.
 */
char *dna_antisense(const char *dna_seq)
{
    //assert(dna_pure_seq(char *dna_seq));
    const int seq_len = strlen(dna_seq);
    char *dna_antisense = (char*)malloc(seq_len + 1);

    for (int i = 0; i < seq_len; ++i)
    {
        if (dna_seq[i] == 'T')
        {
            dna_antisense[seq_len - 1 - i] = 'A';
        }
        else if (dna_seq[i] == 'A')
        {
            dna_antisense[seq_len - 1 - i] = 'T';
        }
        else if (dna_seq[i] == 'G')
        {
            dna_antisense[seq_len - 1 - i] = 'C';
        }
        else
        {
            dna_antisense[seq_len - 1 - i] = 'G';
        }
    }
    dna_antisense[seq_len] = '\0';
    return dna_antisense;
}

/**
 * \brief DNA -> RNA.
 *
 * Start reading when encountering a start codon. Both the DNA sequence and the
 * resulting RNA sequence are in the 5' -> 3' direction.
 * 
 * \param dna_seq    A DNA sequence. 
 * \return           RNA sequence resulting from transcription.
 */
char* transcription(const char* dna_seq)
{
    //assert(dna_pure_seq(const char *dna_seq));
    const int seq_len = strlen(dna_seq);
    char* rna_seq = (char*)malloc(seq_len + 1);

    for (int i = 0; i < seq_len; ++i)
    {
        if (dna_seq[i] == 'T')
        {
            rna_seq[i] = 'U';
        }
        else
        {
            rna_seq[i] = dna_seq[i];
        }
    }
    rna_seq[seq_len] = '\0';
    return rna_seq;
}

/**
 * \brief Translate RNA to an amino acid sequence.
 *
 * \param dna_seq    A RNA sequence. 
 * \return           A sequence of amino acids.
 */
char *translation(const char *rna_seq)
{
    //assert(rna_pure_seq(rna_seq);
    const int n_amino = strlen(rna_seq) / 3;
    char* amino_seq = (char*)malloc(n_amino + 1);

    int i = 0, a = 0;
    while (a < n_amino)
    {
        if (rna_seq[i] == 'U')
        {
            if (rna_seq[i+1] == 'U')
            {
                if (rna_seq[i+2] == 'U')
                {
                    amino_seq[a] = GCODE_UUU;
                }
                else if (rna_seq[i+2] == 'G')
                {
                    amino_seq[a] = GCODE_UUG;
                }
                else if (rna_seq[i+2] == 'C')
                {
                    amino_seq[a] = GCODE_UUC;
                }
                else
                {
                    amino_seq[a] = GCODE_UUA;
                }
            }
            else if (rna_seq[i+1] == 'G')
            {
                if (rna_seq[i+2] == 'U')
                {
                    amino_seq[a] = GCODE_UGU;
                }
                else if (rna_seq[i+2] == 'G')
                {
                    amino_seq[a] = GCODE_UGG;
                }
                else if (rna_seq[i+2] == 'C')
                {
                    amino_seq[a] = GCODE_UGC;
                }
                else
                {
                    amino_seq[a] = GCODE_UGA;
                }
            }
            else if (rna_seq[i+1] == 'C')
            {
                if (rna_seq[i+2] == 'U')
                {
                    amino_seq[a] = GCODE_UCU;
                }
                else if (rna_seq[i+2] == 'G')
                {
                    amino_seq[a] = GCODE_UCG;
                }
                else if (rna_seq[i+2] == 'C')
                {
                    amino_seq[a] = GCODE_UCC;
                }
                else
                {
                    amino_seq[a] = GCODE_UCA;
                }
            }
            else
            {
                if (rna_seq[i+2] == 'U')
                {
                    amino_seq[a] = GCODE_UAU;
                }
                else if (rna_seq[i+2] == 'G')
                {
                    amino_seq[a] = GCODE_UAG;
                }
                else if (rna_seq[i+2] == 'C')
                {
                    amino_seq[a] = GCODE_UAC;
                }
                else
                {
                    amino_seq[a] = GCODE_UAA;
                }
            }
        }
        else if (rna_seq[i] == 'G')
        {
            if (rna_seq[i+1] == 'U')
            {
                if (rna_seq[i+2] == 'U')
                {
                    amino_seq[a] = GCODE_GUU;
                }
                else if (rna_seq[i+2] == 'G')
                {
                    amino_seq[a] = GCODE_GUG;
                }
                else if (rna_seq[i+2] == 'C')
                {
                    amino_seq[a] = GCODE_GUC;
                }
                else
                {
                    amino_seq[a] = GCODE_GUA;
                }
            }
            else if (rna_seq[i+1] == 'G')
            {
                if (rna_seq[i+2] == 'U')
                {
                    amino_seq[a] = GCODE_GGU;
                }
                else if (rna_seq[i+2] == 'G')
                {
                    amino_seq[a] = GCODE_GGG;
                }
                else if (rna_seq[i+2] == 'C')
                {
                    amino_seq[a] = GCODE_GGC;
                }
                else
                {
                    amino_seq[a] = GCODE_GGA;
                }
            }
            else if (rna_seq[i+1] == 'C')
            {
                if (rna_seq[i+2] == 'U')
                {
                    amino_seq[a] = GCODE_GCU;
                }
                else if (rna_seq[i+2] == 'G')
                {
                    amino_seq[a] = GCODE_GCG;
                }
                else if (rna_seq[i+2] == 'C')
                {
                    amino_seq[a] = GCODE_GCC;
                }
                else
                {
                    amino_seq[a] = GCODE_GCA;
                }
            }
            else
            {
                if (rna_seq[i+2] == 'U')
                {
                    amino_seq[a] = GCODE_GAU;
                }
                else if (rna_seq[i+2] == 'G')
                {
                    amino_seq[a] = GCODE_GAG;
                }
                else if (rna_seq[i+2] == 'C')
                {
                    amino_seq[a] = GCODE_GAC;
                }
                else
                {
                    amino_seq[a] = GCODE_GAA;
                }
            }
        }
        else if (rna_seq[i] == 'C')
        {
            if (rna_seq[i+1] == 'U')
            {
                if (rna_seq[i+2] == 'U')
                {
                    amino_seq[a] = GCODE_CUU;
                }
                else if (rna_seq[i+2] == 'G')
                {
                    amino_seq[a] = GCODE_CUG;
                }
                else if (rna_seq[i+2] == 'C')
                {
                    amino_seq[a] = GCODE_CUC;
                }
                else
                {
                    amino_seq[a] = GCODE_CUA;
                }
            }
            else if (rna_seq[i+1] == 'G')
            {
                if (rna_seq[i+2] == 'U')
                {
                    amino_seq[a] = GCODE_CGU;
                }
                else if (rna_seq[i+2] == 'G')
                {
                    amino_seq[a] = GCODE_CGG;
                }
                else if (rna_seq[i+2] == 'C')
                {
                    amino_seq[a] = GCODE_CGC;
                }
                else
                {
                    amino_seq[a] = GCODE_CGA;
                }
            }
            else if (rna_seq[i+1] == 'C')
            {
                if (rna_seq[i+2] == 'U')
                {
                    amino_seq[a] = GCODE_CCU;
                }
                else if (rna_seq[i+2] == 'G')
                {
                    amino_seq[a] = GCODE_CCG;
                }
                else if (rna_seq[i+2] == 'C')
                {
                    amino_seq[a] = GCODE_CCC;
                }
                else
                {
                    amino_seq[a] = GCODE_CCA;
                }
            }
            else
            {
                if (rna_seq[i + 2] == 'U')
                {
                    amino_seq[a] = GCODE_CAU;
                }
                else if (rna_seq[i + 2] == 'G')
                {
                    amino_seq[a] = GCODE_CAG;
                }
                else if (rna_seq[i + 2] == 'C')
                {
                    amino_seq[a] = GCODE_CAC;
                }
                else
                {
                    amino_seq[a] = GCODE_CAA;
                }
            }
        }
        else
        {
            if (rna_seq[i + 1] == 'U')
            {
                if (rna_seq[i + 2] == 'U')
                {
                    amino_seq[a] = GCODE_AUU;
                }
                else if (rna_seq[i + 2] == 'G')
                {
                    amino_seq[a] = GCODE_AUG;
                }
                else if (rna_seq[i + 2] == 'C')
                {
                    amino_seq[a] = GCODE_AUC;
                }
                else
                {
                    amino_seq[a] = GCODE_AUA;
                }
            }
            else if (rna_seq[i + 1] == 'G')
            {
                if (rna_seq[i + 2] == 'U')
                {
                    amino_seq[a] = GCODE_AGU;
                }
                else if (rna_seq[i + 2] == 'G')
                {
                    amino_seq[a] = GCODE_AGG;
                }
                else if (rna_seq[i + 2] == 'C')
                {
                    amino_seq[a] = GCODE_AGC;
                }
                else
                {
                    amino_seq[a] = GCODE_AGA;
                }
            }
            else if (rna_seq[i + 1] == 'C')
            {
                if (rna_seq[i + 2] == 'U')
                {
                    amino_seq[a] = GCODE_ACU;
                }
                else if (rna_seq[i + 2] == 'G')
                {
                    amino_seq[a] = GCODE_ACG;
                }
                else if (rna_seq[i + 2] == 'C')
                {
                    amino_seq[a] = GCODE_ACC;
                }
                else
                {
                    amino_seq[a] = GCODE_ACA;
                }
            }
            else
            {
                if (rna_seq[i + 2] == 'U')
                {
                    amino_seq[a] = GCODE_AAU;
                }
                else if (rna_seq[i + 2] == 'G')
                {
                    amino_seq[a] = GCODE_AAG;
                }
                else if (rna_seq[i + 2] == 'C')
                {
                    amino_seq[a] = GCODE_AAC;
                }
                else
                {
                    amino_seq[a] = GCODE_AAA;
                }
            }
        }
        i += 3;
        ++a;
    }

    /*
    // Slightly faster function than the above, but less flexible
    int i = 0, a = 0;
    while (a < N_AMINO) {
    if (rna_seq[i] == 'U') {                         // U..
        if (rna_seq[i+1] == 'U') {                     // UU.
            if (rna_seq[i+2] == 'U' || rna_seq[i+2] == 'C')
                amino_seq[a] = 'F';                       // UUU, UUC -> Phenylalanine
            else
                    amino_seq[a] = 'L';                     // UUA, UUG -> Leucine
            } else if (rna_seq[i+1] == 'C') {            // UC.
                    amino_seq[a] = 'S';                     // UCA, UCG, UCC, UCU -> Serine
            } else if (rna_seq[i+1] == 'A') {            // UA.
                if (rna_seq[i+2] == 'U' || rna_seq[i+2] == 'C')
                    amino_seq[a] = 'Y';                     // UAU, UAC -> Tyrosine
                else
                    amino_seq[a] = 'Z';                     // UAA, UAG -> STOP
            } else {                                    // UG.
                if (rna_seq[i+2] == 'U' || rna_seq[i+2] == 'C')
                    amino_seq[a] = 'C';                     // UGU, UGC -> Cysteine
                else if (rna_seq[i+2] == 'A')
                    amino_seq[a] = 'Z';                     // UGA -> STOP
                else
                    amino_seq[a] = 'W';                     // UGG -> Tryptophan
            }
        } else if (rna_seq[i] == 'C') {                // C..
            if (rna_seq[i+1] == 'U') {                   // CU.
                amino_seq[a] = 'L';                       // CUA, CUG, CUU, CUC -> Leucine
            } else if (rna_seq[i+1] == 'C') {            // CC.
                amino_seq[a] = 'P';                       // CCA, CCG, CCC, CCU -> Proline
            } else if (rna_seq[i+1] == 'A') {            // CA.
                if (rna_seq[i+2] == 'U' || rna_seq[i+2] == 'C')
                    amino_seq[a] = 'H';                     // CAU, CAC -> Histidine
                else
                    amino_seq[a] = 'Q';                     // CAA, CAG -> Glutamine
            } else {                                    // CG.
                amino_seq[a] = 'R';                       // CGA, CGG, CGC, CGU -> Arginine
            }
        } else if (rna_seq[i] == 'A') {                // A..
            if (rna_seq[i+1] == 'U') {                   // AU.
                if (rna_seq[i+2] == 'G')                       
                    amino_seq[a] = 'M';                     // AUG -> Start codon/Methionine
                else
                    amino_seq[a] = 'I';                     // AUC, AUU, AUA -> Isoleucine
            } else if (rna_seq[i+1] == 'C') {            // AC
                amino_seq[a] = 'T';                       // ACA, ACG, ACC, ACU -> Threonine
            } else if (rna_seq[i+1] == 'A') {            // AA.
                if (rna_seq[i+2] == 'U' || rna_seq[i+2] == 'C')
                    amino_seq[a] = 'N';                     // AAU, AAC -> Asparagine
                else
                    amino_seq[a] = 'K';                     // AAA, AAG -> Lysine
            } else {                                    // AG.
                if (rna_seq[i+2] == 'U' || rna_seq[i+2] == 'C')
                    amino_seq[a] = 'S';                     // AGU, AGC -> Serine
                else
                    amino_seq[a] = 'R';                     // AGA, AGG -> Arginine
            }
        } else {                                      // G..
            if (rna_seq[i+1] == 'U') {                   // GU.
                amino_seq[a] = 'V';                       // GUA, GUU, GUC, GUG -> Valine
            } else if (rna_seq[i+1] == 'C') {            // GC
                amino_seq[a] = 'A';                       // GCA, GCU, GCC, GCG -> Alanine
            } else if (rna_seq[i+1] == 'A') {            // GA.
                if (rna_seq[i+2] == 'U' || rna_seq[i+2] == 'C')
                    amino_seq[a] = 'D';                     // GAU, GAC -> Aspartic acid
                else
                    amino_seq[a] = 'E';                     // GAA, GAG -> Glutamic acid
            } else {                                    // GG.
                amino_seq[a] = 'G';                       // GGA, GGU, GGG, GGC -> Glycine
            }
        }

        i += 3;
        ++a;
    }
    */

    amino_seq[n_amino] = '\0';
    return amino_seq;
}

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
