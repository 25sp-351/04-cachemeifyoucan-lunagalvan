#ifndef CACHE_H
#define CACHE_H

#define CACHE_SIZE 10

typedef struct {
    int entries[CACHE_SIZE];
    int size;
    char policy;
} Cache;

Cache* create_cache(char policy);
void update_cache(Cache *cache, int length);
void free_cache(Cache *cache);

#endif