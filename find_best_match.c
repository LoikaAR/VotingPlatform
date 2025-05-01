#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "find_best_match.h"
#include "build_neighborhood.h"
#include "image.h"

double find_best_match(Image *Ga, Image *Gs, int L, int xs, int ys) {
    // change each level
    int width;
    int heigth;

    double *Ns = build_neighborhood(Gs, L, xs, ys);
    double best_pixel;
    int C = INFINITY;
    for (int x = 0; x < width; x++) {
        for (int y = 0; y < heigth; y++) {
            double *Na = build_neighborhood(Ga, L, x, y);
            int C_new = match_neighborhood(Na, Ns, L);

            if (C_new < C) {
                C = C_new;
                best_pixel = 3 * (x * width + y);
            }
        }
    }
    return best_pixel;
}