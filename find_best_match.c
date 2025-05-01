#include "find_best_match.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "build_neighborhood.h"
#include "image.h"

int find_best_match(Image *sample_pyr, Image *output_pyr, int level, int xs,
                    int ys) {
    int neigh_size = (11 + 9) * 3;

    int *Ns = build_neighborhood(output_pyr, level, xs, ys);

    int best_pixel;
    int C = __INT16_MAX__;
    for (int x = 0; x < sample_pyr[level].width; x++) {
        for (int y = 0; y < sample_pyr[level].height; y++) {
            int *Na = build_neighborhood(sample_pyr, level, x, y);
            int C_new = match_neighborhood(Na, Ns, neigh_size);

            if (C_new < C) {
                C = C_new;
                best_pixel = 3 * (x * sample_pyr[level].width + y);
            }
            free(Na);
        }
    }

    free(Ns);
    return best_pixel;
}