#ifndef PARSER_H
#define PARSER_H

#include <stddef.h>
#include <stdio.h>

typedef struct {
    int length;
    int value;
    int max_count;   /* extra credit: 0 = unlimited */
} Piece;

/* Read price list from `filename`. Returns 0 on success, -1 on error. */
int read_price_list(const char *filename, Piece **pieces_out, size_t *count_out);

/* Read query lengths (one number per line) from `in`.
 * Allocates *queries_out (caller must free), sets *count_out and *max_query_out. */
int read_queries(FILE *in, int **queries_out, size_t *count_out, int *max_query_out);

#endif // PARSER_H