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

// defines entry struct that consists of int k and l and a long value
struct entry {
    int k, l;
    long value;
};

// function that takes to pointers to entries - checks if the first value is bigger (1), equal (0) or smaller (-1) than the second
int comp_entry(const void *p1, const void *p2) {
    const struct entry *e1 = p1;
    const struct entry *e2 = p2;
    if (e1->value < e2->value)
        return -1;
    else
        return e1->value > e2->value;
}

// computes the cube of a long
long cube(long n) {
    return n * n * n;
}

size_t size_table(long n)
/* compute a table size that is large enough to keep all I^3+J^3<n */
{
    return exp(log((double) n) * (2.0 / 3.0)) / 2 + 100;
}

// Merges two subarrays of arr[].
// First subarray is arr[l..m]
// Second subarray is arr[m+1..r]
void merge(long *arr, long l,
           long m, long r) {
    printf("merge(%ld, %ld, %ld) \n", l, m, r);
    long i, j, k;
    long n1 = m - l + 1;
    long n2 = r - m;

    // Create temp arrays
    long L[n1], R[n2];

    // Copy data to temp arrays
    // L[] and R[]
    for (i = 0; i < n1; i++)
        L[i] = arr[l + i];
    for (j = 0; j < n2; j++)
        R[j] = arr[m + 1 + j];

    // Merge the temp arrays back
    // into arr[l..r]
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

    // Copy the remaining elements
    // of L[], if there are any
    while (i < n1) {
        arr[k] = L[i];
        i++;
        k++;
    }

    // Copy the remaining elements of
    // R[], if there are any
    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;
    }
}

int main(int argc, char **argv) {
    long n;
    long cbrt_n;
    char *endptr;
    long i, j;
    long count = 0;
//    struct entry *table;
    long *table;
    long *indices;
    size_t table_size;
    long m = 0;
    long checksum = 0;

    if (argc != 2)
        goto usage;
    // converts str to integer number with base 10
    n = strtol(argv[1], &endptr, 10);
    if (*endptr != '\0')
        goto usage;

    cbrt_n = (long) cbrt(n);

    table_size = size_table(n);
    // allocates memory for the table
//    table = calloc(table_size, sizeof(struct entry));
    table = calloc(table_size, sizeof(long));
    indices = calloc((long) cbrt(n), sizeof(long));
//    printf("%ld \n", (long)cbrt(n));
//    printf("%ld \n", table_size);

    bool value_changed = true;
    // fills the table with the values of i^3+j^3
    // instantiates the table with the required structs
    // TODO: store cube(i) and cube(j) to not calculate it that often
    for (i = 0; cube(i) <= n; i++) {
        if (value_changed) {
            indices[i] = m;
            value_changed = false;
        }
        for (j = i + 1; cube(i) + cube(j) <= n; j++) {
            // create new entry and store it in the table
//            printf("%ld %ld \n", table_size, m);
//            table[m++] = (struct entry) {i, j, cube(i) + cube(j)};
            table[m++] = cube(i) + cube(j);
            value_changed = true;
        }
//        printf("%ld \n", i);
    }
    indices[i] = m;
    printf("done \n");
    assert(m <= table_size);
//    qsort(table, m, sizeof(struct entry), comp_entry);

    for (i = 0; i < table_size; i++) {
        printf("%ld \n", table[i]);
    }

    printf("done \n");

    for (i = 0; i < table_size; i++) {
        printf("%ld \n", table[i]);
    }

    printf("done \n");

    printf("indices: \n");
    for (int i = 0; i < m; ++i) {
        printf("%ld \n", indices[i]);
    }

    // sort table
    // 1st iteration 0, 2, 4, ...
    // 2nd iteration 0, 4, 8, 12, ...
    // 3rd iteration 0, 8, 16, 24, ...
    int increment = 1;
    for (i = 0; i < (long) log2(cbrt_n); i++) {
        printf("Iteration: %ld of %ld n=%ld \n", i + 1, (long) log2(cbrt_n), n);
        for (j = 0; j < cbrt_n - 2 * increment; j += 2 * increment) {
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

    // iterate over the table and check if the next entry is the same as the current one
//    for (i = 1; i < m; i++) {
//        if (table[i - 1].value == table[i].value) {
//            count++;
//            checksum += table[i].value;
//            while (i < m - 1 && table[i].value == table[i + 1].value)
//                i++;
//        }
//    }
    printf("%ld Ramanujan numbers up to %ld, checksum=%ld\noccupation=%ld, size=%ld\n", count, n, checksum, m,
           table_size);
    printf("Memory usage: >=%ld\n", table_size * sizeof(struct entry));
    return 0;

    usage:
    fprintf(stderr, "usage: %s <n>\n", argv[0]);
    exit(1);
}
