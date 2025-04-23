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
const int upper_neighborhood_px = 50;
const int num_levels = 4;
const int base_width = 4;
const int base_heigth = 4;
// TODO: fix with real value
// each channel is represented
const int nr_pixels = 3 * base_width * base_heigth * 2;

// TODO: fill pixels array in the correct way
double *build_neighborhood(double *G, int L, int x, int y) {
    // those values change for each level
    int offset;
    int width, heigth;

    float *pixels = malloc(nr_pixels * sizeof(float*));
    // iterate the neighborhood
    for (int i, j; i,j < neighborhood_px; i+=3, j+=3) {
        // Check: rgb encoding?
        // row index
        int xi = (x + i) % width;
        // column index
        int yi = (y + j) % heigth;
        // append right pixel (3 channels) to the pixels array
        pixels[i] = G[offset + xi + width * yi];
        pixels[i+1] = G[offset + xi + width * yi + 1];
        pixels[i+2] = G[offset + xi + width * yi + 2];
    }
    // upper neighborhood
    if (L < sizeof(G)/(sizeof(float*) * 3)) {
        for (int i, j; i, j < upper_neighborhood_px; i+=3, j+=3) {
            // Check: rgb encoding?
            // row index
            int xi = (x / 2 + i) % width;
            // column index
            int yi = (y / 2 + j) % heigth;
            // append right pixel (3 channels) to the pixels array
            pixels[i] = G[offset + xi + width * yi];
            pixels[i+1] = G[offset + xi + width * yi + 1];
            pixels[i+2] = G[offset + xi + width * yi + 2];
        }
    }

    return pixels;
}

