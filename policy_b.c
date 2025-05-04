#include "cache.h"
#include <stdlib.h>
#include <stdbool.h>

#define CAPACITY 1024

typedef struct Entry {
    int length, value, choice;
    struct Entry *prev, *next;
} Entry;

static Entry *head, *tail;
static size_t cur_size;

void cache_init(int max_length) {
    (void)max_length;  /* not used here */
    head = tail = NULL;
    cur_size = 0;
}

bool cache_get(int length, int *value_out, int *choice_out) {
    for (Entry *e = head; e; e = e->next) {
        if (e->length == length) {
            /* move to front */
            if (e != head) {
                if (e->prev) e->prev->next = e->next;
                if (e->next) e->next->prev = e->prev;
                if (e == tail) tail = e->prev;
                e->next = head;
                head->prev = e;
                e->prev = NULL;
                head = e;
            }
            *value_out  = e->value;
            *choice_out = e->choice;
            return true;
        }
    }
    return false;
}

void cache_put(int length, int value, int choice) {
    /* if already in cache, update + move to front */
    for (Entry *e = head; e; e = e->next) {
        if (e->length == length) {
            e->value  = value;
            e->choice = choice;
            if (e != head) {
                if (e->prev) e->prev->next = e->next;
                if (e->next) e->next->prev = e->prev;
                if (e == tail) tail = e->prev;
                e->next = head;
                head->prev = e;
                e->prev = NULL;
                head = e;
            }
            return;
        }
    }

    /* new entry */
    Entry *e = malloc(sizeof *e);
    if (!e) return;
    e->length = length;
    e->value  = value;
    e->choice = choice;
    e->prev = NULL;
    e->next = head;
    if (head) head->prev = e;
    head = e;
    if (!tail) tail = e;
    cur_size++;

    /* evict if over capacity */
    if (cur_size > CAPACITY) {
        Entry *old = tail;
        tail = tail->prev;
        tail->next = NULL;
        free(old);
        cur_size--;
    }
}

void cache_destroy(void) {
    Entry *e = head;
    while (e) {
        Entry *n = e->next;
        free(e);
        e = n;
    }
    head = tail = NULL;
    cur_size = 0;
}