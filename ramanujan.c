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

long cube(long n) {
    return n * n * n;
}

size_t size_res_table(long n)
{
    int size = ((int) log10((double) n)) + 5;
    return 1 << size;
}

size_t hash(long key, size_t hash_size)
/* assumes that hash_size is a power of two */
{
    return key & (hash_size - 1);
}

long *lookup(long key, long *table, size_t table_size)
/* comment */
{
    long pos = key;
    long *pp = table + hash(key, table_size);
    for (;*pp != 0; pp = table + hash(++pos, table_size)) {
        if (*pp == key) {
            return pp;
        }
    }
    return pp;
}

long calcUpperBound(long lowerBound, int window) {
    return (long) (lowerBound == 0 ? 30000000000 : ((double) lowerBound * (1.04 / (1-0.86*exp(-0.31*window)))));
}

long min(long x, long y) {
    return x < y ? x : y;
}

long max(long x, long y) {
    return x < y ? y : x;
}

long calcMinJ(long lowerBound, long i) {
    if (lowerBound == 0 || cube(i) > lowerBound) {
        return i+1;
    }
    return max(i+1, (long) (double) cbrt((double) lowerBound - (double) cube(i)));
}

void zeroOutTable(long *table, size_t table_size) {
    for (int i = 0; i < table_size; ++i) {
        *(table + i) = 0;
    }
}

int main(int argc, char **argv) {
    long n;
    char *endptr;
    long i, j;
    long count = 0;
    long *candidate_table;
    size_t candidate_table_size;
    long *res_table;
    size_t res_table_size;
    long checksum = 0;

    if (argc != 2)
        goto usage;
    n = strtol(argv[1], &endptr, 10);
    if (*endptr != '\0')
        goto usage;
    candidate_table_size = 1<<23;
    candidate_table = calloc(candidate_table_size, 8);
    res_table_size = size_res_table(n);
    res_table = calloc(res_table_size, 8);

    long lowerBound;
    long upperBound = 0;
    for (int k = 0; upperBound < n; ++k) {
        lowerBound = upperBound;
        upperBound = min(n, calcUpperBound(lowerBound, k));

        for (i = 0; cube(i) <= upperBound; i++) {
            for (j = calcMinJ(lowerBound, i); cube(i) + cube(j) <= upperBound; j++) {
                long sum = cube(i) + cube(j);

                if (sum >= lowerBound) {
                    long *pos = lookup(sum, candidate_table, candidate_table_size);
                    if (*pos == sum) {
                        long *pos_res = lookup(sum, res_table, res_table_size);
                        if (*pos_res != sum) {
                            count++;
                            checksum += sum;
                            *pos_res = sum;
                        }
                    } else {
                        *pos = sum;
                    }
                }
            }
        }
        zeroOutTable(candidate_table, candidate_table_size);
    }
    printf("%ld Ramanujan numbers up to %ld, checksum=%ld\n", count, n, checksum);
    printf("Memory usage: >=%ld\n", candidate_table_size * 8 + res_table_size * 8);
    return 0;

    usage:
    fprintf(stderr, "usage: %s <n>\n", argv[0]);
    exit(1);
}
