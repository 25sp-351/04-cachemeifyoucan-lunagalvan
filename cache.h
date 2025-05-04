#ifndef CACHE_H
#define CACHE_H

#include <stdbool.h>

/* Initialize the cache 
 * `max_length` is the largest rod length we’ll ever ask for. */
void cache_init(int max_length);

/* Try to look up subproblem `length`. Returns true if found, 
 * filling *value_out and *choice_out. */
bool cache_get(int length, int *value_out, int *choice_out);

/* Store the result for subproblem `length`. */
void cache_put(int length, int value, int choice);

/* Tear down any cache data. */
void cache_destroy(void);

#endif // CACHE_H