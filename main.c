#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "rod_cutting.h"
#include "cache.h"

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <price_file> <policy (A or B)>\n", argv[0]);
        return EXIT_FAILURE;
    }

    const char *filename = argv[1];
    char policy = argv[2][0];
    if (policy != 'A' && policy != 'B') {
        fprintf(stderr, "Invalid policy. Use 'A' for LRU or 'B' for MRU/LIFO.\n");
        return EXIT_FAILURE;
    }

    int piece_count;
    Piece *pieces = read_price_file(filename, &piece_count);
    if (!pieces) {
        fprintf(stderr, "Error reading price file.\n");
        return EXIT_FAILURE;
    }

    Cache *cache = create_cache(policy);
    int rod_length;
    while (scanf("%d", &rod_length) == 1) {
        printf("Rod Length: %d\n", rod_length);
        solve_rod_cutting(rod_length, pieces, piece_count, cache);
    }

    free(pieces);
    free_cache(cache);
    return EXIT_SUCCESS;
}