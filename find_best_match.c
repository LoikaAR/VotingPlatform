#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "find_best_match.h"
#include "build_neighborhood.h"

double find_best_match(double *Ga, double *Gs, int L, int xs, int ys) {
    // change each level
    int width;
    int heigth;

    double *Ns = build_neighborhood(Gs, L, xs, ys);
    double best_pixel;
    int C = INFINITY;
    for (int x = 0; x < width; x++) {
        for (int y = 0; y < heigth; y++) {
            double *Na = build_neighborhood(Ga, L, x, y);
            double C_new = match_neighborhood(Na, Ns);

            if (C_new < C) {
                C = C_new;
                best_pixel = x * width + y;
            }
        }
    }
    return best_pixel;
}
