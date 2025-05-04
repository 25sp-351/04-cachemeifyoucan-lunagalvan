#include <stdio.h>
#include <stdlib.h>
#include "parser.h"
#include "rodcut.h"
#include "cache.h"

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <price-file>\n", argv[0]);
        return EXIT_FAILURE;
    }

    /* 1) Read the one‐time price list */
    Piece *pieces = NULL;
    size_t piece_count = 0;
    if (read_price_list(argv[1], &pieces, &piece_count) != 0) {
        perror("read_price_list");
        return EXIT_FAILURE;
    }

    /* 2) Read *all* query lengths from stdin */
    int *queries = NULL;
    size_t query_count = 0;
    int max_query = 0;
    if (read_queries(stdin, &queries, &query_count, &max_query) != 0) {
        perror("read_queries");
        free(pieces);
        return EXIT_FAILURE;
    }

    /* 3) Initialize cache up to the largest requested length */
    cache_init(max_query);

    /* 4) For each query, compute & print */
    for (size_t i = 0; i < query_count; i++) {
        int L = queries[i];
        int total = cut_rod(L, pieces, piece_count);

        int *counts = NULL;
        int rem = 0;
        reconstruct_cuts(L, pieces, piece_count, &counts, &rem);

        for (size_t j = 0; j < piece_count; j++) {
            if (counts[j] > 0) {
                printf("%d @ %d = %d\n",
                       counts[j],
                       pieces[j].length,
                       counts[j] * pieces[j].value);
            }
        }
        printf("Remainder: %d\n", rem);
        printf("Value: %d\n", total);
        free(counts);
    }

    /* 5) Clean up */
    cache_destroy();
    free(pieces);
    free(queries);
    return EXIT_SUCCESS;
}