#ifndef RODCUT_H
#define RODCUT_H

#include <stddef.h>
#include "parser.h"

#define NO_CHOICE (-1)

/* Compute best total for `length`. Uses the global cache. */
int cut_rod(int length,
            const Piece *pieces,
            size_t piece_count);

/* Reconstruct how many of each piece to use, plus the leftover. */
void reconstruct_cuts(int length,
                      const Piece *pieces,
                      size_t piece_count,
                      int **counts_out,
                      int *remainder_out);

#endif // RODCUT_H