#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

// Assume that a maximum of 10^20 cores will be used (that's 10 billion billion)
// The lower this number, the quicker initalization is.
// The higher, the more certain you can be 
// that you won't accidentally miss out on efficient prime distributions
#define MAX_PROCESSORS 20

int  blind_phi(int x);
bool coprime(int a, int b);
int *find_all_primes(int b);
int *find_coprimes(int x);
int  find_optimal_p_factors(int max_phi, int current_x, int *primes, int prime_offset);
int  find_optimal_phi_ratio(int n);
int  inverse_blind_phi(int a);
int  phi(int x, int *primes);

/*
    Find all primes between 2 and b

    The function returns an allocated array that ends with a -1
*/
int *find_all_primes(int b) {
    int *primes = malloc(sizeof(int) * blind_phi(b));
    primes[0] = -1;

    for (int n=2; n<=b; n++) {
        bool is_prime = true;
        int i = 0;

        while (primes[i] != -1) {
            int p = primes[i];

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
    Calculate phi based on known prime numbers
*/
int phi(int x, int *primes) {
    int n     = x;
    int phi_x = 1;

    for (int i=0; primes[i]!=-1; i++) {
        int p           = primes[i];
        int occurrences = 0;

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
    Look for a factor of numbers as high as possible
    where phi(x) is still lower than the number
*/
int find_optimal_p_factors(int max_phi, int current_x, int *primes, int prime_offset) {
    int optimal_factor = 1;
    
    for (int i=prime_offset; primes[i]!=-1; i++) {
        int p = primes[i];
        if (phi(current_x * p, primes) > max_phi) {
            break;
        }

        int factor = find_optimal_p_factors(max_phi, current_x*p, primes, i);
        if (p * factor > optimal_factor) {
            optimal_factor = p * factor;
        }
    }

    return optimal_factor;
}

/*
    Calculate whether two numbers a coprime or not.
*/
bool coprime(int a, int b) {
    while (a > 1) {
        int temp = b;
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
int *find_coprimes(int x) {
    int *primes = find_all_primes(x);
    int phi_x   = phi(x, primes);

    free(primes);

    int *coprimes = malloc(sizeof(int) * (phi_x + 1));
    int cursor = 0;

    for (int i=1; i<x; i++) {
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
int blind_phi(int x) {
    int n     = x;
    int phi_x = 1;
    
    for (int p=2; p<=n; p++) {
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

/*
    Find the first x for which blind_phi(x)=a

    If there is no such x, the function returns zero.
*/
int inverse_blind_phi(int a) {
    int x = a+1;
    int maximum = 2*a*a;

    while (x < maximum) {
        if (blind_phi(x) == a) {
            return x;
        }
        x++;
    }
    return 0;
}

/*
    Calculate the number of cores to distribute the prime numbers in
    to go through the numbers as efficiently as possible.
*/
int find_optimal_prime_distribution(int n) {
    int maximum = 2*n*n;
    if (2*n >= MAX_PROCESSORS) {
        maximum = MAX_PROCESSORS * n;
    }

    for (int x=maximum; x>1; x--) {
        if (blind_phi(x) <= n) {
            return x;
        }
    }

    return 1;
}
