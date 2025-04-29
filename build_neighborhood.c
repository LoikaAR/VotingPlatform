#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "build_neighborhood.h"
#include "image_struct.h"

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
int *build_neighborhood(int *G, int L, int x, int y) {
    // those values change for each level
    int width = get_width(L);
    int height = get_height(L);
    int offset = get_offset(L);

    // total ints for 11 pixels in neighborhood
    // and 9 pixels in upper neighborhood
    int neighborhood[11][2] = {
        {-6, -6}, {-6, -3}, {-6, 0}, {-6, 3},
        {-3, -6}, {-3, -3}, {-3, 0}, {-3, 3}, {-3, 6},
        {0, -6}, {0, -3}
    };

    int upper_neighborhood[9][2] = {
        {-3, -3}, {-3, 0}, {-3, 3},
        {0, -3}, {0, 0},  {0, 3},
        {3, -3}, {3, 0},  {3, 3}
    };

    int num_pixels = 60;    
    int *pixels = malloc(num_pixels * sizeof(int));

    // iterate the neighborhood
    // change loop
    int max_iterations = 3 * neighborhood_size;

    for (int i = 0; i < max_iterations; i+=3) {
        int dx = neighborhood[i][0];
        int dy = neighborhood[i][1];
        if (x + dx < 0 || y + dy < 0) continue;

        int xi = (x + dx) % width;
        int yi = (y + dy) % height;
        int index = (xi + width * yi) * 3;
        // append right pixel (3 channels) to the pixels array
        pixels[i] = G[offset + index];
        pixels[i+1] = G[offset + index + 1];
        pixels[i+2] = G[offset + index + 2];
    }
    // upper neighborhood
    if (L < MAX_LEVELS - 1) {
        int max_iterations = 3 * upper_neighborhood_size;
        // informations for upper levels
        int up_width = get_width(L + 1);
        int up_height = get_height(L + 1);
        int up_offset = get_offset(L + 1);
        for (int i = 0; i < max_iterations; i+=3) {
            // Check: rgb encoding?
            // row indexes
            int dx = upper_neighborhood[i][0];
            int dy = upper_neighborhood[i][1];
            if (x + dx < 0 || y + dy < 0) continue;

            int xi = (x / 2 + dx) % up_width;
            int yi = (y / 2 + dy) % up_height;
            int index = (xi + up_width * yi) * 3;
            // append right pixel (3 channels) to the pixels array
            pixels[i] = G[up_offset + index];
            pixels[i+1] = G[up_offset + index + 1];
            pixels[i+2] = G[up_offset + index + 2];
        }
    }

    return pixels;
}

int match_neighborhood(int *Na, int *Ns, int l) {
    int res = 0;
    for (int i = 0; i < l; i++) {
        res += (Na[i] - Ns[i]) * (Na[i] - Ns[i]);
    }    
    return res;
}
