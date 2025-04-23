#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "build_neighborhood.h"

// is it a constant?
const int neighborhood = 100;
const int nr_pixels = 1000;
// understand x, y values and neighborhood dimension

double *build_neighborhood(double *G, double *L, int x, int y) {
    float *pixels = malloc(nr_pixels * sizeof(float*));
    // iterate the neighborhood
    for (int i, j; i,j < neighborhood; i++, j++) {
        
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