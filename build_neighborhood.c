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
const int neighborhood_px = 100;
const int upper_neighborhood_px = 50;
// TODO: fix with real value
// each channel is represented
const int nr_pixels = 3 * BASE_WIDTH * BASE_HEIGHT * 2;

// neighborhood contains pixel's relative 
// coordinates respective to actual pixel
int neighborhood[][2] = {
    {-2, -2}, {-2, -1}, {-2, 0}, {-2, 1},
    {-1, -2}, {-1, -1}, {-1, 0}, {-1, 1}, {-1, 2},
     {0, -2}, {0, -1}
};

int upper_neighborhood[][2] = {
    {-1, -1}, {-1, 0}, {-1, 1},
     {0, -1}, {0, 0},  {0, 1},
     {1, -1}, {1, 0},  {1, 1}
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
    // TODO: calculate offset for each level, width and heigth
    int width = get_width(L);
    int height = get_height(L);
    int offset = get_offset(L);

    int num_pixels = (sizeof(neighborhood)/sizeof(neighborhood[0]) 
        + sizeof(upper_neighborhood)/sizeof(upper_neighborhood[0])) * 3;
        
    double *pixels = malloc(num_pixels * sizeof(double));
    
    // iterate the neighborhood
    for (int i = 0, j = 0; i, j < neighborhood_px; i+=3, j+=3) {
        // Check: rgb encoding?
        // row index
        int xi = (x + i) % width;
        // column index
        int yi = (y + j) % height;
        // append right pixel (3 channels) to the pixels array
        pixels[i] = G[offset + xi + width * yi];
        pixels[i+1] = G[offset + xi + width * yi + 1];
        pixels[i+2] = G[offset + xi + width * yi + 2];
    }
    // upper neighborhood
    if (L < sizeof(G)/(sizeof(double*) * 3)) {
        for (int i, j; i, j < upper_neighborhood_px; i+=3, j+=3) {
            // Check: rgb encoding?
            // row index
            int xi = (x / 2 + i) % width;
            // column index
            int yi = (y / 2 + j) % height;
            // append right pixel (3 channels) to the pixels array
            pixels[i] = G[offset + xi + width * yi];
            pixels[i+1] = G[offset + xi + width * yi + 1];
            pixels[i+2] = G[offset + xi + width * yi + 2];
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

/*
int x, y
int L
int rgb_clamp
double *Image
*/