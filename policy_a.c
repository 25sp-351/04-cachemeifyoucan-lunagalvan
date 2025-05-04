#include "cache.h"
#include <stdlib.h>
#include <stdbool.h>

static bool *valid;
static int  *values;
static int  *choices;
static int   cache_size;

void cache_init(int max_length) {
    cache_size = max_length + 1;
    valid   = calloc(cache_size, sizeof(bool));
    values  = malloc(cache_size * sizeof(int));
    choices = malloc(cache_size * sizeof(int));
}

bool cache_get(int length, int *value_out, int *choice_out) {
    if (length < 0 || length >= cache_size || !valid[length]) return false;
    *value_out  = values[length];
    *choice_out = choices[length];
    return true;
}

void cache_put(int length, int value, int choice) {
    if (length < 0 || length >= cache_size) return;
    valid[length]   = true;
    values[length]  = value;
    choices[length] = choice;
}

void cache_destroy(void) {
    free(valid);
    free(values);
    free(choices);
}