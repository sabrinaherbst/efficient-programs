# Efficient Programs
- Sabrina Herbst
- Stefan Öhlwerther
- Florian Schabasser

## Project
This projects tries to optimise the original version of the implementation of Ramanujan numbers.
The goal is to make it as efficient as possible and decrease Memory usage to 100MB max.

## Files
- `ramanujan.c` - version using a hashtable
- `ramasort.c` - version using a sorted array

## Branches
We used different branches for different versions. The main branch contains the final version.

Description of the branches:
* `main` - final version
* `original` - original version
* `ramasort-struct-elimination` - first optimisation for ramasort, eliminating the struct
* `ramasort-mergesort` - replaces qsort with the merge command of mergesort
* `ramasort-mergesort-loop` - makes minor loop changes to make it more efficient
* `ramasort-memory-opt` - optimises memory usage
* `ramasort-memory-lowerbound` - last optimisation updating the index based on the lower bound

## Results
The following shows our results using n^13. 

| File      | Original Runtime | Optimised Runtime | Optimised Memory |
|-----------|------------------|-------------------| ---------------- |
| Ramanujan | TODO             | TODO              | TODO             |
| Ramasort  | 25,30s           | 9,8085s           | 100MB            |