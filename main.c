#include <stdio.h>
#include <bsp.h>
#include <stdlib.h>
#include "config.h"
#include "initialize.c"
#include "sieve.c"

static unsigned int P;

void spmd() {
    bsp_begin( P );

    // INITIALIZATION PROCESS
    // Determine where to iterate.
    // Also, gather the first few prime numbers.
    // 
    BIG_INT *primes      = find_all_primes(P);
    BIG_INT  x           = find_optimal_p_factors(P, 1, primes, 0);
    if (P==1) { // Hardcode if there's only 1 core
        x = 2;
    }
    BIG_INT *coprimes    = find_coprimes(x);
    BIG_INT b            = coprimes[bsp_pid()];

    free(primes);
    free(coprimes);

    // Get ready for finding prime numbers
    struct primeSlice *slice = buildStandardSlice(bsp_pid(), bsp_nprocs(), x);
    BIG_INT numbers[1024];

    // Initialization complete
    printf("Core %u has initialized as %dn+%d\n", bsp_pid(), x, b);

    // There's no point in syncing at the end of initialization
    // as no information needs to be exchanged, but I like the
    // idea that the programs all start looking for prime numbers
    // at the same time.
    bsp_sync(); // comment this line if you want to optimize!

    bsp_end();
}

int main( int argc, char ** argv ) {
    unsigned int available_cores;

    return 0;

    printf( "How many threads do you want started? There are %u cores available.\n", bsp_nprocs() );    
    fflush( stdout );
    scanf( "%u", &available_cores );

    BIG_INT *primes = find_all_primes(available_cores);
    BIG_INT x       = find_optimal_p_factors(available_cores, 1, primes, 0);

    BIG_INT phi_x   = phi(x, primes);

    printf("If you have %u cores available, it is best to use %d of them.\n", available_cores, phi_x);

    free(primes);
    P = phi_x;

    // Start threads
    if( P == 0 || P > bsp_nprocs() ) {
        fprintf( stderr, "Cannot start %u threads.\n", P );
        return EXIT_FAILURE;
    }
    bsp_init( &spmd, argc, argv );
    spmd();

    printf("Function has ended.\n");
    
    return EXIT_SUCCESS;
}