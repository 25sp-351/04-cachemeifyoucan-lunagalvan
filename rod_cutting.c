#include "rod_cutting.h"
#include <stdio.h>
#include <stdlib.h>

Piece* read_price_file(const char *filename, int *piece_count) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Failed to open price file");
        return NULL;
    }

    Piece *pieces = malloc(sizeof(Piece) * 100);
    *piece_count = 0;

    while (fscanf(file, "%d, %d, %d", &pieces[*piece_count].length, &pieces[*piece_count].value, &pieces[*piece_count].max_cuts) == 3) {
        (*piece_count)++;
    }
    
    fclose(file);
    return pieces;
}

void solve_rod_cutting(int rod_length, Piece *pieces, int piece_count, Cache *cache) {
    int remaining = rod_length, total_value = 0;
    int used[piece_count];
    for (int i = 0; i < piece_count; i++) used[i] = 0;

    for (int i = 0; i < piece_count && remaining > 0; i++) {
        while (remaining >= pieces[i].length && (pieces[i].max_cuts == 0 || used[i] < pieces[i].max_cuts)) {
            remaining -= pieces[i].length;
            total_value += pieces[i].value;
            used[i]++;
            printf("1 @ %d = %d\n", pieces[i].length, pieces[i].value);
            update_cache(cache, pieces[i].length);
        }
    }

    printf("Remainder: %d\n", remaining);
    printf("Value: %d\n\n", total_value);
}