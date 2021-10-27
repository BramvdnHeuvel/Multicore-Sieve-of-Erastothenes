#include <stdio.h>
#include <bsp.h>
#include <stdlib.h>
#include "config.h"
#include "initialize.c"
#include "sieve.c"


static unsigned int P;
static BIG_INT limit;

void spmd() {
    bsp_begin( P );

    // INITIALIZATION PROCESS
    // Determine where to iterate.
    // Also, gather the first few prime numbers.
    // 
    BIG_INT *primes      = find_all_primes(P+1);
    BIG_INT  x           = find_optimal_p_factors(P, 1, primes, 0);
    if (P==1) { // Hardcode if there's only 1 core
        x = 2;
    }
    BIG_INT *coprimes    = find_coprimes(x);
    BIG_INT b            = coprimes[bsp_pid()];

    free(primes);
    free(coprimes);

    // Get ready for finding prime numbers
    struct primeSlice  *s = buildStandardSlice(bsp_pid(), bsp_nprocs(), x);
    struct filterSlice *f = newFilterSlice(x, b, limit);

    // Initialization complete
    // printf("Core %u has initialized as %dn+%d\n", bsp_pid(), x, b);

    // There's no point in syncing at the end of initialization
    // as no information needs to be exchanged, but I like the
    // idea that the programs all start looking for prime numbers
    // at the same time.
    // bsp_sync(); // uncomment this line if you think that that feels better too

    int empty_filter_slices = 0;

    while (empty_filter_slices < 2*bsp_nprocs() || f->b != b) {
        filterSliceWithPrimeSlice(f, s);
        // if (bsp_pid() == 0) {
        //     printFilterSlice(f);
        //     printPrimeSlice(s);
        // }

        bsp_push_reg(f, sizeof(struct filterSlice));
        bsp_sync();

        bsp_get((bsp_pid() - 1) % bsp_nprocs(), f, 0, f, sizeof(struct filterSlice));
        bsp_sync();

        // Check if we got a filterSlice back that
        // we created ourselves - in that case,
        // we know that it's been filtered completely
        if (f->b == b) {
            for (int i=0; i<f->length; i++) {
                BIG_INT p = f->nums[i];

                if (p > 0) {
                    printf("%d ", p);
                    insertPrimeNumber(p, s);
                }
            }
            // if (f->length > 0) {
            //     printf("\n");
            // }

            refreshFilterSlice(f);
        }

        // If there are no more numbers, 
        // we've reached the limit.
        if (f->length == 0) {
            empty_filter_slices++;
        } else {
            empty_filter_slices = 0;
        }
    }

    // Free the created memory
    // This may cause memory leaks if the prime slices expand too much,
    // but it doesn't need a fix that badly - yet.
    free(f);
    free(s);

    bsp_end();
}

int main( int argc, char ** argv ) {
    unsigned int available_cores = 4;
    limit = 10000000;

    // printf( "How many threads do you want started? There are %u cores available.\n", bsp_nprocs() );    
    // fflush( stdout );
    // scanf( "%u", &available_cores );

    BIG_INT *primes = find_all_primes(available_cores+1);
    BIG_INT x       = find_optimal_p_factors(available_cores, 1, primes, 0);

    BIG_INT phi_x   = phi(x, primes);

    printf("If you have %u cores available, it is best to use %d of them.\n", available_cores, phi_x);

    // printf("How far do you want to look for prime numbers?\n");
    // fflush( stdout );
    // scanf( "%lu", &limit );

    free(primes);
    P = phi_x;

    // Start threads
    if( P == 0 || P > bsp_nprocs() ) {
        fprintf( stderr, "Cannot start %u threads.\n", P );
        return EXIT_FAILURE;
    }
    bsp_init( &spmd, argc, argv );
    spmd();

    printf("\n");
    
    return EXIT_SUCCESS;
}