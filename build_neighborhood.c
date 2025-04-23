#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "build_neighborhood.h"

// is it a constant?
// to add meaningful numbers
// understand x, y values and neighborhood dimension
// L is the level of the image of the gaussian filter
// TODO: right values for constants
const int neighborhood_px = 100;
const int num_levels = 4;
const int base_width = 256;
const int base_heigth = 256;
// TODO: fix with real value
const int nr_pixels = base_width * base_heigth * 2;

double *build_neighborhood(double *G, int L, int x, int y) {
    float *pixels = malloc(nr_pixels * sizeof(float*));
    // iterate the neighborhood
    for (int i, j; i,j < neighborhood_px; i++, j++) {
        pixels[i] = G[L + (x + i)];
    }
}


/*
def build_neighborhood(G, L, x, y):
    pixels = []
    for i, j in neighborhood:
        pixels.append(G[L][(x + i) % G[L].shape[0], (y + j) % G[L].shape[1]])
    if L < len(G) - 1:
        for i, j in upper_neighborhood:
            pixels.append(G[L + 1][(x // 2 + i) % G[L + 1].shape[0], (y // 2 + j) % G[L + 1].shape[1]])
    return np.array(pixels)
*/
