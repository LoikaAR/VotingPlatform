#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "find_best_match.h"
#include "build_neighborhood.h"
#include "image.h"

int find_best_match(Image *Ga, Image *Gs, int L, int xs, int ys) {
    // change each level
    int width = Ga[L].width;
    int heigth = Ga[L].height;
    int neigh_size = (11 + 9) * 3; 

    double *Ns = build_neighborhood(Gs, L, xs, ys);
    int best_pixel;
    int C = __INT16_MAX__; // changed from INFINITY
    for (int x = 0; x < width; x++) {
        for (int y = 0; y < heigth; y++) {
            double *Na = build_neighborhood(Ga, L, x, y);
            int C_new = match_neighborhood(Na, Ns, neigh_size);

            if (C_new < C) {
                C = C_new;
                best_pixel = 3 * (x * width + y);
            }
        }
    }
    // printf("%d best pixel\n", best_pixel);
    return best_pixel;
}