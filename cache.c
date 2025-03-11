#include "cache.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

Cache* create_cache(char policy) {
    Cache *cache = (Cache*)malloc(sizeof(Cache));
    cache->size = 0;
    cache->policy = policy;
    return cache;
}

void update_cache(Cache *cache, int length) {
    if (cache->size < CACHE_SIZE) {
        cache->entries[cache->size++] = length;
    } else {
        if (cache->policy == 'A') { // LRU
            memmove(&cache->entries[0], &cache->entries[1], sizeof(int) * (CACHE_SIZE - 1));
            cache->entries[CACHE_SIZE - 1] = length;
        } else { // MRU/LIFO
            cache->entries[CACHE_SIZE - 1] = length;
        }
    }
}

void free_cache(Cache *cache) {
    free(cache);
}