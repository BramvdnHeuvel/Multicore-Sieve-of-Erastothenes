#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

// Assume that a maximum of 10^20 cores will be used (that's 10 billion billion)
// The lower this number, the quicker initalization is.
// The higher, the more certain you can be 
// that you won't accidentally miss out on efficient prime distributions
#define MAX_PROCESSORS 20

bool coprime(int a, int b);
int *find_coprimes(int x);
int  find_optimal_phi_ratio(int n);
int  inverse_phi(int a);
int  phi(int x);

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
    int *coprimes = malloc(sizeof(int) * x);
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
int phi(int x) {
    int *coprimes = find_coprimes(x);
    int primes = 0;

    for (int i=0; i<=x; i++) {
        if (coprimes[i] == -1) {
            primes = i;
            break;
        }
    }

    free(coprimes);
    return primes;
}

/*
    Find the first x for which phi(x)=a

    If there is no such x, the function returns zero.
*/
int inverse_phi(int a) {
    int x = a+1;
    int maximum = 2*a*a;

    while (x < maximum) {
        if (phi(x) == a) {
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
        if (phi(x) <= n) {
            return x;
        }
    }

    return 1;

    int phi_x   = 2;
    int x       = 1;

    for (int i=2; i<maximum; i++) {
        int new_phi_x   = phi(i);
        int new_x       = i;

        if (new_phi_x <= n && new_x > x) {
            x       = new_x;
            phi_x   = new_phi_x;
        }
    }

    return x;
}
