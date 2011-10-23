#ifndef WELL1024_H_
#define WELL1024_H_

#define WELL_MAT3POS(t,v) (v^(v>>t))
#define WELL_MAT3NEG(t,v) (v^(v<<(-t)))
#define WELL_IDEN(v) (v)

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>
#include <string.h>
#include <math.h>
#include <limits.h>

// For C++ compilers:
#ifdef __cplusplus
extern "C"
{
#endif

/** The state of a well1024 random number generator. */
typedef struct
{
    unsigned int state[32];
    unsigned int state_n;
    unsigned int seed; // The initial seed used to initiate the generator
    // For non-uniform random numbers;
    int have_next_normal;
    double next_normal;
}
well1024;

/** Julienne Walker's function (from www.eternallyconfuzzled.com) to get an unsigned int from time(). */
unsigned int time_seed()
{
    time_t now = time(0);
    unsigned char *p = (unsigned char*)&now;
    unsigned int seed = 0;

    // Knuth's method (TAOCP vol 2).
    size_t i = 0;
    for (; i < sizeof(now); ++i)
    {
        seed = seed * (UCHAR_MAX + 2U) + p[i];
    }
    return seed;
}

/** Seed the generator with an unsigned integer. */
void well1024_init(well1024 *rng, unsigned int seed)
{
    assert(seed != 0);
    rng->have_next_normal = FALSE;
    rng->seed = seed;
    rng->state_n = 0;
    rng->state[0] = rng->seed & 0xffffffffU;

    int i = 1;
    for (; i < 32; ++i)
    {
        rng->state[i] = (69069 * rng->state[i - 1]) & 0xffffffffU;
    }
}

/** Initialize with time using time_seed(). Return the seed used. */
unsigned int well1024_init_time(well1024 *rng)
{
    unsigned int seed = 0;
    
    do
    {
        seed = time_seed();
    }
    while (seed == 0);

    well1024_init(rng, seed);

    return seed;
}

/** Return a double in the [0, 1) range. */
double well1024_next_double(well1024 *rng)
{
    unsigned int *const state = rng->state;
    const unsigned int state_n = rng->state_n;
    const unsigned int z0 = state[(state_n + 31) & 0x0000001fUL];
    const unsigned int z1 = WELL_IDEN(state[state_n]) ^ WELL_MAT3POS(8, state[(state_n + 3) & 0x0000001fUL]);
    const unsigned int z2 = WELL_MAT3NEG(-19, state[(state_n+24) & 0x0000001fUL]) ^ WELL_MAT3NEG(-14, state[(state_n + 10) & 0x0000001fUL]);

    state[state_n] = z1 ^ z2;
    state[(state_n + 31) & 0x0000001fUL] = WELL_MAT3NEG(-11,z0) ^ WELL_MAT3NEG(-7,z1) ^ WELL_MAT3NEG(-13,z2) ;
    rng->state_n = (state_n + 31) & 0x0000001fUL;

    return ((double)state[rng->state_n] * 2.32830643653869628906e-10);
}

// UNIFORM DISTRIBUTION

/** Return an integer in the [0, b) semi-closed range. */
int well1024_next_int(well1024 *rng, int b)
{
    return (int)(well1024_next_double(rng) * b);
}

unsigned int well1024_next_uint(well1024 *rng, int b)
{
    return (unsigned int)(well1024_next_double(rng) * b);
}

int well1024_next_max_int(well1024 *rng)
{
    return (int)(well1024_next_double(rng) * INT_MAX);
}

unsigned int well1024_next_max_uint(well1024 *rng)
{
    return (int)(well1024_next_double(rng) * UINT_MAX);
}

// NON-UNIFORM DISTRIBUTIONS

/** Return a double following the exponential distribution (between 0 and 1). */
double well1024_next_exp(well1024 *rng)
{
    double r;
    
    do
    {
        r = well1024_next_double(rng);
    }
    while (r == 0.0);

    return -log(r);
}

/** Return a double following the normal distribution (between 0 and 1). */
double well1024_next_normal(well1024 *rng)
{
    if (rng->have_next_normal == TRUE)
    {
        rng->have_next_normal = FALSE;
        return rng->next_normal;
    }
    else
    {
        double n1, n2, s;
        
        do
        {
            /* Two numbers in the (-1.0, 1.0) range. */
            n1 = 2.0 * well1024_next_double(rng) - 1.0;
            n2 = 2.0 * well1024_next_double(rng) - 1.0;
            s = n1 * n1 + n2 * n2;
        }
        while (s >= 1 || s == 0);

        const double m = sqrt(-2.0 * log(s) / s);
        rng->next_normal = n2 * m;
        rng->have_next_normal = TRUE;

        return n1 * m;
    }
}

/** Return an integer from a poisson distribution given lambda. \f$O(\lambda)\f$. */
int well1024_next_poisson(well1024 *rng, double lambda)
{
    const double l = pow(2.71828182845904523536, -lambda);
    double p = 1.0;
    int k = 0;
    
    do
    {
        k++;
        p *= well1024_next_double(rng);
    }
    while (p > l);

    return k - 1;
}


// UTILS

/** Return the state of the generator as a string. */
char* well1024_to_string(well1024 *rng)
{
    char* to_string = (char*)malloc(12 * 32);
    char buffer[12];

    int i = 0;
    for (; i < 32; ++i)
    {
        sprintf(buffer,"%u ", rng->state[i]);
        strcat(to_string, buffer);
    }
    return to_string;
}

#ifdef __cplusplus
}
#endif

#endif
