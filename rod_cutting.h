#ifndef ROD_CUTTING_H
#define ROD_CUTTING_H

#include "cache.h"

typedef struct {
    int length;
    int value;
    int max_cuts;
} Piece;

Piece* read_price_file(const char *filename, int *piece_count);
void solve_rod_cutting(int rod_length, Piece *pieces, int piece_count, Cache *cache);

#endif