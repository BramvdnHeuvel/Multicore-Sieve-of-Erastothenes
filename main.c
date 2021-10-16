#include <stdio.h>
#include "initialize.c"

int main(void) {
    unsigned int P;

    printf( "How many cores do you have available?\n");    
    fflush( stdout );
    scanf( "%u", &P );

    printf("Finding prime numbers below %u... ", P);
    int *primes = find_all_primes(P);
    printf("Complete!\n");

    printf("Finding optimal x value... ");
    int x       = find_optimal_p_factors(P, 1, primes, 0);
    printf("Complete!\n");

    int phi_x   = phi(x, primes);

    printf("If you have %u cores available, it is best to use only %d of them.\n", P, phi_x);
    printf("Numbers coprime to %d are the following:\n", x);

    int *coprimes = find_coprimes(x);

    for (int i=0; coprimes[i]!=-1; i++) {
        printf("%dn+%d   ", x, coprimes[i]);
    }

    printf("\n");

    free(primes);
    free(coprimes);
}