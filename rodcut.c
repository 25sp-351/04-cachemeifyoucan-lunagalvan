#include "rodcut.h"
#include "cache.h"
#include <stdlib.h>
#include <stdbool.h>

/* Globals for the unbounded, cached version */
static const Piece *unb_pieces;
static size_t       unb_count;

static bool use_bounded = false;

static int best_value_unb(int rem, int *choice_out) {
    if (rem <= 0) return 0;

    int cv, cc;
    if (cache_get(rem, &cv, &cc)) {
        if (choice_out) *choice_out = cc;
        return cv;
    }

    int best = 0, best_choice = NO_CHOICE;
    for (size_t i = 0; i < unb_count; i++) {
        int plen = unb_pieces[i].length;
        if (plen <= rem) {
            int val = unb_pieces[i].value + best_value_unb(rem - plen, NULL);
            if (val > best) {
                best = val;
                best_choice = (int)i;
            }
        }
    }

    cache_put(rem, best, best_choice);
    if (choice_out) *choice_out = best_choice;
    return best;
}



static void reconstruct_unb(int length,
                            const Piece *pieces,
                            size_t piece_count,
                            int **counts_out,
                            int *remainder_out)
{
    int *counts = calloc(piece_count, sizeof(int));
    int rem = length;

    while (rem > 0) {
        int val, choice;
        if (!cache_get(rem, &val, &choice) || choice == NO_CHOICE)
            break;
        counts[choice]++;
        rem -= pieces[choice].length;
    }

    *counts_out    = counts;
    *remainder_out = rem;
}


/*---------------------------------------------------------
 * Bounded, bottom‐up DP
 *--------------------------------------------------------*/

static int cut_bounded(int length,
                       const Piece *pieces,
                       size_t piece_count,
                       int **counts_out,
                       int *remainder_out)
{
    size_t n = piece_count;
    int L = length;

    int **dp   = malloc((n+1)*sizeof(int*));
    int **take = malloc((n+1)*sizeof(int*));
    if (!dp || !take) exit(EXIT_FAILURE);

    for (size_t i = 0; i <= n; i++) {
        dp[i]   = calloc(L+1, sizeof(int));
        take[i] = calloc(L+1, sizeof(int));
        if (!dp[i] || !take[i]) exit(EXIT_FAILURE);
    }

    for (size_t i = 1; i <= n; i++) {
        int plen = pieces[i-1].length;
        int pval  = pieces[i-1].value;
        int maxc  = pieces[i-1].max_count;  /* 0 = unlimited */

        for (int l = 0; l <= L; l++) {
            /* start with taking zero of piece i−1 */
            dp[i][l]   = dp[i-1][l];
            take[i][l] = 0;

            /* compute how many we COULD take */
            int upper = plen > 0 ? l / plen : 0;
            if (maxc > 0 && maxc < upper) upper = maxc;

            for (int k = 1; k <= upper; k++) {
                int val = dp[i-1][l - k*plen] + k * pval;
                if (val > dp[i][l]) {
                    dp[i][l]   = val;
                    take[i][l] = k;
                }
            }
        }
    }

    /* reconstruct counts */
    int *counts = calloc(n, sizeof(int));
    int rem = L;
    for (size_t i = n; i >= 1; i--) {
        int k = take[i][rem];
        counts[i-1] = k;
        rem -= k * pieces[i-1].length;
    }

    int total = dp[n][L];

    /* free DP tables */
    for (size_t i = 0; i <= n; i++) {
        free(dp[i]);
        free(take[i]);
    }
    free(dp);
    free(take);

    *counts_out    = counts;
    *remainder_out = rem;
    return total;
}

int cut_rod(int length,
            const Piece *pieces,
            size_t piece_count)
{
    if (!use_bounded) {
        for (size_t i = 0; i < piece_count; i++) {
            if (pieces[i].max_count > 0) {
                use_bounded = true;
                break;
            }
        }
    }

    if (!use_bounded) {
        unb_pieces = pieces;
        unb_count  = piece_count;
        return best_value_unb(length, NULL);
    } else {
        /* bounded: we don't use cache here */
        int *dummy_counts, dummy_rem;
        int val = cut_bounded(length, pieces, piece_count, &dummy_counts, &dummy_rem);
        free(dummy_counts);
        return val;
    }
}

void reconstruct_cuts(int length,
                      const Piece *pieces,
                      size_t piece_count,
                      int **counts_out,
                      int *remainder_out)
{
    if (!use_bounded) {
        reconstruct_unb(length, pieces, piece_count, counts_out, remainder_out);
    } else {
        cut_bounded(length, pieces, piece_count, counts_out, remainder_out);
    }
}