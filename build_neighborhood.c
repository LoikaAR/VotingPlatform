#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "build_neighborhood.h"

#define BASE_WIDTH 4
#define BASE_HEIGHT 4
#define MAX_LEVELS 4

// is it a constant?
// to add meaningful numbers
// understand x, y values and neighborhood dimension
// L is the level of the image of the gaussian filter
// TODO: right values for constants
const int neighborhood_size = 11;
const int upper_neighborhood_size = 9;
// TODO: fix with real value
// each channel is represented
const int nr_pixels = 3 * BASE_WIDTH * BASE_HEIGHT * 2;

// neighborhood contains pixel's relative 
// coordinates respective to actual pixel
// here counting the 3 channels too
int neighborhood[][2] = {
    {-6, -6}, {-6, -3}, {-6, 0}, {-6, 3},
    {-3, -6}, {-3, -3}, {-3, 0}, {-3, 3}, {-3, 6},
     {0, -6}, {0, -3}
};

int upper_neighborhood[][2] = {
    {-3, -3}, {-3, 0}, {-3, 3},
     {0, -3}, {0, 0},  {0, 3},
     {3, -3}, {3, 0},  {3, 3}
};

// Get width and height of level L
int get_width(int L) { return BASE_WIDTH >> L; }
int get_height(int L) { return BASE_HEIGHT >> L; }

// Get offset (in pixels) to level L
int get_offset(int L) {
    int offset = 0;
    for (int i = 0; i < L; i++) {
        offset += get_width(i) * get_height(i) * 3;
    }
    return offset;
}

// TODO: fill pixels array in the correct way
double *build_neighborhood(double *G, int L, int x, int y) {
    // those values change for each level
    int width = get_width(L);
    int height = get_height(L);
    int offset = get_offset(L);

    int num_pixels = (sizeof(neighborhood)/sizeof(neighborhood[0]) 
        + sizeof(upper_neighborhood)/sizeof(upper_neighborhood[0])) * 3;
        
    double *pixels = malloc(num_pixels * sizeof(double));
    
    // iterate the neighborhood
    // change loop
    int i = neighborhood[0][0];
    int j = neighborhood[0][1];
    int max_iterations = i + 3 * neighborhood_size;

    for (; i < max_iterations; i+=3, j+=3) {
        // Check: rgb encoding?
        // index assigning
        // not going out of bound
        if (i - x < 0) continue;
        if (j - y < 0) continue;

        int xi = (x + i) % width;
        int yi = (y + j) % height;
        int index = (xi + width * yi) * 3;
        // append right pixel (3 channels) to the pixels array
        pixels[i] = G[offset + index];
        pixels[i+1] = G[offset + index + 1];
        pixels[i+2] = G[offset + index + 2];
    }
    // upper neighborhood
    if (L < sizeof(G)/(sizeof(double*) * 3)) {
        for (; i < max_iterations; i+=3, j+=3) {
            // Check: rgb encoding?
            // row indexes
            if (i - x < 0) continue;
            if (j - y < 0) continue;

            int xi = (x / 2 + i) % width;
            int yi = (y / 2 + j) % height;
            int index = (xi + width * yi) * 3;
            // append right pixel (3 channels) to the pixels array
            pixels[i] = G[offset + index];
            pixels[i+1] = G[offset + index + 1];
            pixels[i+2] = G[offset + index + 2];
        }
    }

    return pixels;
}

double match_neighborhood(double *Na, double *Ns, int l) {
    double res;
    for (int i = 0; i < l; i++) {
        res += (Na[i] - Ns[i]) * (Na[i] - Ns[i]);
    }    
    return res;
}
