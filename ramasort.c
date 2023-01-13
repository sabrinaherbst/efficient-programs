/* Ramanujan numbers are numbers that can be formed by adding two
   cubes of integer numbers in two (or more) different ways, i.e.
   i^3+j^3 = k^3+l^3
   See <http://www.durangobill.com/Ramanujan.html>

   This program counts the Ramanujan numbers up to a given
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <malloc.h>
#include <assert.h>
#include <stdbool.h>

// computes the cube of a long
long cube(long n) {
    return n * n * n;
}

size_t size_table(long n)
/* compute a table size that is large enough to keep all I^3+J^3<n */
{
    return (size_t) exp(log((double) n) * (2.0 / 3.0)) / 2 + 100;
}

// Merges two sub-arrays of arr[].
// First subarray is arr[l..m]
// Second subarray is arr[m+1..r]
void merge(long *arr, long l, long m, long r) {
    long i, j, k;
    long n1 = m - l + 1;
    long n2 = r - m;

    // Create temp arrays
    long *L = malloc(n1 * sizeof(long));
    long *R = malloc(n2 * sizeof(long));

    // Copy data to temp arrays
    // L[] and R[]
    for (i = 0; i < n1; i++)
        L[i] = arr[l + i];
    for (j = 0; j < n2; j++)
        R[j] = arr[m + 1 + j];

    // Merge the temp arrays back into arr[l..r]
    // Initial index of first subarray
    i = 0;

    // Initial index of second subarray
    j = 0;

    // Initial index of merged subarray
    k = l;
    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            arr[k] = L[i];
            i++;
        } else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }

    // Copy the remaining elements of L[] if there are any
    while (i < n1) {
        arr[k] = L[i];
        i++;
        k++;
    }

    // Copy the remaining elements of R[] if there are any
    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;
    }

    free(L);
    free(R);

}

int main(int argc, char **argv) {
    long n;
    long cbrt_n;
    char *endptr;
    long i, j;
    long count = 0;
    long *table;
    long *indices;
    size_t table_size;
    long m = 0;
    long checksum = 0;

    if (argc != 2)
        goto usage;
    // converts str to an integer number with base 10
    n = strtol(argv[1], &endptr, 10);
    if (*endptr != '\0')
        goto usage;

    cbrt_n = (long) cbrt((double) n);

    table_size = 7500000;
    // allocates memory for the table
    table = calloc(table_size, sizeof(long));

    long n_indices;
    double exp = log2((double) cbrt_n);
    n_indices = exp - trunc(exp) == 0 ? (long) cbrt_n : (long) pow(2, trunc(exp) + 1);
    indices = calloc(n_indices + 1, sizeof(long));

    long inc = 20000;
    long lower = -1, upper = inc;
    long sum, cube_i;

    while (upper <= n + inc) {

        // clear table
        memset(table, 0, table_size);
        memset(indices, 0, n_indices + 1);
        m = 0;

        bool value_changed = true;
        // fills the table with the values of i^3+j^3
        for (i = 0; (cube_i = cube(i)) < n; i++) {
            //        printf("i: %ld m: %ld \n", i, m);
            if (value_changed) {
                indices[i] = m;
                value_changed = false;
            } else {
                for (long k = i; k < n_indices + 1; k++) {
                    indices[k] = m;
                }
                break;
            }

            if (cube_i + cube(i + 1) < lower) {
                j = (long) cbrt((double) lower - (double) cube_i) + 1;
            } else {
                j = i + 1;
            }
            for (; (sum = (cube_i + cube(j))) <= n; j++) {
                if (sum <= upper && sum > lower) {
                    table[m++] = sum;
                    value_changed = true;
                }

            }
        }

        assert(m <= table_size);

        long increment = 1;
        for (i = 0; i < (long) log2((double) n_indices); i++) {
            for (j = 0; j <= n_indices - 2 * increment; j += 2 * increment) {
                merge(table, indices[j], indices[j + increment] - 1, indices[j + increment * 2] - 1);
            }
            increment *= 2;
        }

        // iterate over the table and check if the next entry is the same as the current one
        for (i = 1; i < m; i++) {
            if (table[i - 1] == table[i]) {
                count++;
                checksum += table[i];
                while (i < m - 1 && table[i] == table[i + 1])
                    i++;
            }
        }

        if (m <= table_size / 3) {
            inc *= 2;
        }

        lower = upper;
        upper = upper + inc;
    }

    printf("%ld Ramanujan numbers up to %ld, checksum=%ld\noccupation=%ld, size=%ld\n", count, n, checksum, m,
           table_size);
    printf("Memory usage: >=%ld\n", table_size * sizeof(long) + (n_indices + 1) * sizeof(long));
    return 0;

    usage:
    fprintf(stderr, "usage: %s <n>\n", argv[0]);
    exit(1);
}
