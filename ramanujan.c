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

// represents a node with a reference to the next one, value and count
struct node {
  struct node *next;
  long value;
  long count;
};

// calculates the cube of a long
long cube(long n)
{
  return n*n*n;
}

size_t size_table(long n)
/* compute the table size so it is not too densely or too sparsely occupied 
   and is a power of 2 */
{
  return 1<<(long)(log((double)n)*(2.0/(3.0*log(2.0))));
}

size_t hash(long key, size_t hash_size)
/* assumes that hash_size is a power of two */
{
    // Efficient version of modulo
  return key&(hash_size-1);
}

struct node **lookup(long key, struct node **table, size_t table_size)
/* goes through the table and searches for the node with value key and returns a pointer to the pointer of that object */
{
  struct node **pp = table+hash(key,table_size);
  // iterate through list
  for (; *pp!=NULL; pp = &((*pp)->next))
    if ((*pp)->value == key)
      return pp;
  return pp;
}

int main(int argc, char **argv)
{
  long n;
  char *endptr;
  long i,j;
  long count=0;
  struct node **table;
  size_t table_size;
  long occupation=0;
  long checksum = 0;

  if (argc != 2)
    goto usage;
  // converts this argument to an int with base 10
  n = strtol(argv[1],&endptr,10);
  if (*endptr != '\0')
    goto usage;

  table_size = size_table(n);
  // allocate memory for the table
  table = calloc(table_size,sizeof(struct node*));

    // TODO precalculate cube 0 to n and save the values.
    for (i=0; cube(i)<=n; i++) {
        // TODO calculate cube(i)+cube(j) only once
        for (j=i+1; cube(i)+cube(j)<=n; j++) {
            long sum = cube(i)+cube(j);
            // TODO hold HashTable small by removing values smaller than i (dont forget to free memory).
            // TODO check table size.
            struct node **sumnodepp = lookup(sum,table,table_size);
            // check if already exists in table
            if (*sumnodepp != NULL) {
                // TODO: do the if before -> then we do not need to increment the count that many times
                // if so increase the count
                (*sumnodepp)->count++;
                if ((*sumnodepp)->count==2) {
                    count++;
                    checksum+=sum;
                }
            } else {
                // create new node
                // TODO multiple nodes of one list allocated far away -> Cache misses?!
                struct node *new = malloc(sizeof(struct node));
                new->next = NULL;
                new->value = sum;
                new->count = 1;
                // set node to correct position
                *sumnodepp = new;
                occupation++;
            }
        }
    }
  printf("%ld Ramanujan numbers up to %ld, checksum=%ld\noccupation=%ld, size=%ld\n",count,n,checksum,occupation,table_size);
  printf("Memory usage: >=%ld\n",table_size*sizeof(struct node*)+occupation*sizeof(struct node));
  return 0;

 usage:
  fprintf(stderr,"usage: %s <n>\n",argv[0]);
  exit(1);
}
