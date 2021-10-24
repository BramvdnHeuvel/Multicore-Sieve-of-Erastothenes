#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "config.h"

// Defined functions
BIG_INT  blind_phi(BIG_INT x);
bool     coprime(BIG_INT a, BIG_INT b);
BIG_INT *find_all_primes(BIG_INT b);
BIG_INT *find_coprimes(BIG_INT x);
BIG_INT  find_optimal_p_factors(BIG_INT n, BIG_INT current_x, BIG_INT *primes, BIG_INT prime_offset);
BIG_INT  phi(BIG_INT x, BIG_INT *primes);

/*
    Find all primes between 2 and b

    The function returns an allocated array that ends with a -1
*/
BIG_INT *find_all_primes(BIG_INT b) {
    BIG_INT *primes = malloc(sizeof(BIG_INT) * blind_phi(b));
    primes[0] = -1;

    for (BIG_INT n=2; n<=b; n++) {
        bool is_prime = true;
        BIG_INT i = 0;

        while (primes[i] != -1) {
            BIG_INT p = primes[i];

            if (n % p == 0) {
                is_prime = false;
                break;
            }

            i++;
        }

        if (is_prime) {
            primes[i]   =  n;
            primes[i+1] = -1;
        }
    }

    return primes;
}

/*
    Calculate phi(x). The list of primes can be the output of function `find_all_primes`
*/
BIG_INT phi(BIG_INT x, BIG_INT *primes) {
    BIG_INT n     = x;
    BIG_INT phi_x = 1;

    for (BIG_INT i=0; primes[i]!=-1; i++) {
        BIG_INT p           = primes[i];
        int     occurrences = 0;

        while (n % p == 0) {
            occurrences++;
            n = n / p;

            if (occurrences == 1) {
                phi_x = phi_x * (p-1);
            } else {
                phi_x = phi_x * p;
            }
        }
    }

    if (n != 1) {
        printf("WARNING: phi(%d) could not entirely break up!\nRemainder:\t%d\nOutput:\t%d\n", x, n, phi_x);
    }
    return phi_x;
}

/*
    Find an x for a given n such that phi(x) <= n
*/
BIG_INT find_optimal_p_factors(BIG_INT n, BIG_INT current_x, BIG_INT *primes, BIG_INT prime_offset) {
    int optimal_factor = 1;
    
    for (int i=prime_offset; primes[i]!=-1; i++) {
        int p = primes[i];
        if (phi(current_x * p, primes) > n) {
            break;
        }

        int factor = find_optimal_p_factors(n, current_x*p, primes, i);
        if (p * factor > optimal_factor) {
            optimal_factor = p * factor;
        }
    }

    return optimal_factor;
}

/*
    Calculate whether two numbers a coprime or not.
*/
bool coprime(BIG_INT a, BIG_INT b) {
    while (a > 1) {
        BIG_INT temp = b;
        b = a;
        a = temp % a;
    }
    return a;
}

/*
    Find all numbers in {1, ..., x-1} that are coprime with x.

    The function returns a malloc'd array of x integers. The list
    of coprime numbers ends with a -1
*/
BIG_INT *find_coprimes(BIG_INT x) {
    BIG_INT *primes = find_all_primes(x);
    BIG_INT phi_x   = phi(x, primes);

    free(primes);

    BIG_INT *coprimes = malloc(sizeof(int) * (phi_x + 1));
    BIG_INT cursor = 0;

    for (BIG_INT i=1; i<x; i++) {
        if (coprime(i, x)) {
            coprimes[cursor] = i;
            cursor++;
        }
    }
    coprimes[cursor] = -1;

    return coprimes;
}

/*
    Calculate how many numbers lower than x are coprime with x
*/
BIG_INT blind_phi(BIG_INT x) {
    BIG_INT n     = x;
    BIG_INT phi_x = 1;
    
    for (BIG_INT p=2; p<=n; p++) {
        int occurrences = 0;

        // p is a prime number
        while (n % p == 0) {
            occurrences++;
            n = n / p;

            if (occurrences == 1) {
                phi_x = phi_x * (p-1);
            } else {
                phi_x = phi_x * p;
            }
        }
    }

    if (n != 1) {
        printf("WARNING: blind_phi(%d) could not entirely break up!\nRemainder:\t%d\nOutput:\t%d\n", x, n, phi_x);
    }
    return phi_x;
}

