#include "config.h"
#include "initialize.h"

// ==============================================
//            Prime slice structure
// ==============================================
struct primeSlice {
    BIG_INT            nums[1024];
    int                found;
    int                max_total;
    struct primeSlice *next;
};

// All functions in this file
void insertPrimeNumber(BIG_INT p, struct primeSlice *primes);
struct primeSlice *newPrimeSlice(void);

/*
    Create a new primeSlice structure.
*/
struct primeSlice *newPrimeSlice(void) {
    struct primeSlice *slice = malloc(sizeof(struct primeSlice));

    slice->found = 0;
    slice->next  = NULL;
    slice->max_total = sizeof slice->nums / sizeof slice->nums[0];

    return slice;
}

/*
    Insert a prime number into 
*/
void insertPrimeNumber(BIG_INT p, struct primeSlice *primes) {
    while (primes->next) {
        primes = primes->next;
    }
    int found = primes->found;

    // If it's full, add a new one.
    if (found == primes->max_total) {
        primes->next = newPrimeSlice();
        primes       = primes->next;
        found        = 0;
    }

    primes->nums[found] = p;
    primes->found = found + 1;
}

/*
    Return a primeSlice that contains the prime numbers below `n`
    that are meant for this process.
*/
struct primeSlice *buildStandardSlice(BIG_INT pid, BIG_INT total_pids, BIG_INT n) {
    BIG_INT *primes = find_all_primes(n);
    BIG_INT  cursor = total_pids - 1;

    struct primeSlice *slice = newPrimeSlice();

    for (BIG_INT i=0; primes[i]!=-1; i++) {
        BIG_INT p = primes[i];

        if (coprime(p, n)) {
            if (cursor == pid) {
                insertPrimeNumber(p, slice);
            }

            if (cursor == 0) {
                cursor = total_pids;
            }
            cursor--;
        }
    }

    free(primes);
    return slice;
}

void printPrimeSlice(struct primeSlice *slice) {
    while (slice != NULL) {
        printf("{\n\tfound    \t: %d\n\tmax_total\t: %d\n\tnums     \t: [ ", slice->found, slice->max_total);

        for (int i=0; i<slice->found; i++) {
            printf("%d ", slice->nums[i]);
        }

        printf("]\n\tnext\t\t: ");
        if (slice->next == NULL) {
            printf("NULL");
        } else {
            printf("see below...");
        }
        printf("\n}\n");

        slice = slice->next;
    }
}
