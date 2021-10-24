#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "config.h"

/*
    Calculate phi(x). This function is slow but doesn't require an array of
    all prime numbers below x.
*/
BIG_INT  blind_phi(BIG_INT x);

/*
    Return whether a and b are coprime.
*/
bool coprime(BIG_INT a, BIG_INT b);

/*
    Find all prime numbers less than or equal to b.
*/
BIG_INT *find_all_primes(BIG_INT b);

/*
    Find all numbers below x that are coprime with x.
*/
BIG_INT *find_coprimes(BIG_INT x);

/*
    Find an x for a given n such that phi(x) <= n

    All values beyond `n` are values used for recurion. Leave them as the following values:
        current_x       = 1
        primes          =   an array of prime numbers below n
        prime_offset    = 0
*/
BIG_INT  find_optimal_p_factors(BIG_INT n, BIG_INT current_x, BIG_INT *primes, BIG_INT prime_offset);

/*
    Calculate phi(x). The list of primes can be the output of function `find_all_primes`
*/
BIG_INT  phi(BIG_INT x, BIG_INT *primes);