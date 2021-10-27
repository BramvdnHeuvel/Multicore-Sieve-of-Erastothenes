#include "config.h"
#include "initialize.h"
#include <math.h>

// ==============================================
//            Prime slice structure
// ==============================================

// Sorta linked list of prime numbers
struct primeSlice {
    BIG_INT            nums[1024];
    int                found;
    int                max_total;
    struct primeSlice *next;
};

// Slice of integers that we'll look at for each prime number
struct filterSlice {
    BIG_INT             nums[8192];
    int                 length;     // How many numbers are currently stored
    int                 max_total;  // How many it can store in total
    BIG_INT             max_value;  // The highest value currently stored
    BIG_INT             x;
    BIG_INT             b;
    BIG_INT             limit;      // How far is determined to iterate
    double              max_prime;  // The supremum of which prime numbers could be relevant
};

// All functions in this file
void insertPrimeNumber(BIG_INT p, struct primeSlice *primes);
struct primeSlice *newPrimeSlice(void);
struct primeSlice *buildStandardSlice(BIG_INT pid, BIG_INT total_pids, BIG_INT n);
void printPrimeSlice(struct primeSlice *slice);
struct filterSlice *newFilterSlice(BIG_INT x, BIG_INT b, BIG_INT limit);
void fillFilterSlice(struct filterSlice *f, BIG_INT start);
void refreshFilterSlice(struct filterSlice *f);
void filterSliceWithPrime(struct filterSlice *f, BIG_INT p);

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

        if (pid == 0) {
            // Since it's on initialization,
            // we want the prime number
            // to be printed in stdout
            printf("%d ", p);
        }

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

void printFilterSlice(struct filterSlice *f) {
    printf("{\n\tlength\t\t: %d\n\tmax_value\t: %d\n\tmax_prime\t: %lf\n\tx\t\t: %d\n\tb\t\t: %d\n\tnums\t: [ ", f->length, f->max_value, f->max_prime, f->x, f->b);
    for (int i=0; i<f->length; i++) {
        printf("%d ", f->nums[i]);
    }
    printf("]\n}\n");
}

/* Create a filterSlice that is ready to look at the first few values */
struct filterSlice *newFilterSlice(BIG_INT x, BIG_INT b, BIG_INT limit) {
    struct filterSlice *f = malloc(sizeof(struct filterSlice));
    f->max_total          = sizeof f->nums / sizeof f->nums[0];

    f->limit = limit;
    f->x     = x;
    f->b     = b;

    x++;
    fillFilterSlice(f, x);
}

/*
    Fill a filter slice as far as possible.
*/
void fillFilterSlice(struct filterSlice *f, BIG_INT start) {
    BIG_INT max_value = (start * start - start) / f->x;

    if (max_value > f->max_total) {
        max_value = f->max_total;
    }

    // Start at value n_start such that
    //
    //    (n_start-1) * x + b < start <= n_start * x + b
    BIG_INT x       = f->x;
    BIG_INT b       = f->b;
    BIG_INT n_start = (start + (x - 1) - b) / x;

    for (int i=0; i<max_value; i++) {
        f->nums[i]  = (n_start + i) * x + b;

        if (f->nums[i] > f->limit) {
            f->nums[i] = 0;
            max_value  = i;
            break;
        }
    }

    if (max_value > 0) {
        f->length       = max_value;
        f->max_value    = f->nums[max_value-1] + 1;
        f->max_prime    = sqrt (f->max_value) + 0.001;
    } else {
        f->length = 0;
    }

    return;
}

/* Refresh a filterSlice with new numbers to look at */
void refreshFilterSlice(struct filterSlice *f) {
    fillFilterSlice(f, f->max_value + 1);
}

void filterSliceWithPrimeSlice(struct filterSlice *f, struct primeSlice *s) {
    while (s != NULL) {
        for (int i=0; i<s->found; i++) {
            BIG_INT p = s->nums[i];

            if (p < f->max_prime) {
                filterSliceWithPrime(f, p);
            } else {
                break;
            }
        }

        if (s->nums[s->found-1] > f->max_prime) {
            break;
        }
        s = s->next;
    }
}

/* Remove all values from the filterSlice that are divisible by prime number p */
void filterSliceWithPrime(struct filterSlice *f, BIG_INT p) {
    for (int i=0; i<f->length; i++) {
        if (f->nums[i] > 0 && f->nums[i] % p == 0) {

            // nums[i] is divisible by p
            // Hence nums[i+n*p] ==> nums[i] + n*p is divisble by p
            for (int j=0; i+j*p<f->length; j++) {
                f->nums[i+j*p] = 0;
            }

            break;
        }
    }
}
