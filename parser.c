#define _GNU_SOURCE
#include "parser.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define INITIAL_CAPACITY 16

int read_price_list(const char *filename, Piece **pieces_out, size_t *count_out) {
    FILE *f = fopen(filename, "r");
    if (!f) return -1;

    size_t cap = INITIAL_CAPACITY, cnt = 0;
    Piece *arr = malloc(cap * sizeof *arr);
    char *line = NULL;
    size_t len = 0;

    while (getline(&line, &len, f) != -1) {
        char *p = line;
        while (isspace((unsigned char)*p)) p++;
        if (*p == '\0') continue;

        char *endp;
        long plen = strtol(p, &endp, 10);
        if (endp == p || *endp != ',') continue;

        p = endp + 1;
        while (isspace((unsigned char)*p)) p++;
        long pval = strtol(p, &endp, 10);
        if (endp == p) continue;

        /* optional third field: max_count */
        int maxc = 0;
        if (*endp == ',') {
            p = endp + 1;
            while (isspace((unsigned char)*p)) p++;
            long tmp = strtol(p, &endp, 10);
            if (endp != p) maxc = (int)tmp;
        }

        if (cnt == cap) {
            cap *= 2;
            arr = realloc(arr, cap * sizeof *arr);
        }
        arr[cnt].length    = (int)plen;
        arr[cnt].value     = (int)pval;
        arr[cnt].max_count = maxc;
        cnt++;
    }

    free(line);
    fclose(f);
    *pieces_out = arr;
    *count_out  = cnt;
    return 0;
}

int read_queries(FILE *in, int **queries_out, size_t *count_out, int *max_query_out) {
    size_t cap = INITIAL_CAPACITY, cnt = 0;
    int *arr = malloc(cap * sizeof *arr);
    int maxq = 0;

    char *line = NULL;
    size_t len = 0;
    while (getline(&line, &len, in) != -1) {
        char *p = line;
        while (isspace((unsigned char)*p)) p++;
        if (*p == '\0') continue;

        char *endp;
        long q = strtol(p, &endp, 10);
        if (endp == p) continue;

        if (cnt == cap) {
            cap *= 2;
            arr = realloc(arr, cap * sizeof *arr);
        }
        arr[cnt++] = (int)q;
        if (q > maxq) maxq = (int)q;
    }
    free(line);

    *queries_out    = arr;
    *count_out      = cnt;
    *max_query_out  = maxq;
    return 0;
}