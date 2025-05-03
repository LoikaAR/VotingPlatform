#include "find_best_match.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "image.h"
#include "neighborhood.h"

int find_best_match(Image *sample_pyr, Image *output_pyr, int level,
                    int total_levels, int xs, int ys) {
    int neigh_size = (12 + 9) * 3;

    int *Ns = build_neighborhood(output_pyr, level, total_levels, xs, ys);

    int best_pixel = 0;
    int C = __INT_MAX__;
    for (int x = 0; x < sample_pyr[level].width; x++) {
        for (int y = 0; y < sample_pyr[level].height; y++) {
            int *Na = build_neighborhood(sample_pyr, level, total_levels, x, y);
            int C_new = match_neighborhood(Na, Ns, neigh_size);

            if (C_new < C) {
                C = C_new;
                best_pixel = (y * sample_pyr[level].width + x);
            }

            free(Na);
        }
    }

    free(Ns);
    return best_pixel;
}