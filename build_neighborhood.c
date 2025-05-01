#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "build_neighborhood.h"
#include "image.h"

// TODO: Determine values
#define BASE_WIDTH 4
#define BASE_HEIGHT 4
#define MAX_LEVELS 4

// neighborhood contains pixel's relative 
// coordinates respective to actual pixel
// here counting the 3 channels too
int neighborhood[][2] = {
    {-6, -6}, {-6, -3}, {-6, 0}, {-6, 3},
    {-3, -6}, {-3, -3}, {-3, 0}, {-3, 3}, 
    {-3, 6}, {0, -6}, {0, -3}
};

int upper_neighborhood[][2] = {
    {-3, -3}, {-3, 0}, {-3, 3},
    {0, -3}, {0, 0},  {0, 3},
    {3, -3}, {3, 0},  {3, 3}
};

/**
 * @brief Builds neighborhood of pixel (x, y) at given level in a Gaussian pyramid.
 *
 * This function returns the neighborhood of a specific pixel at level L in the pyramid, 
 * including the pixel's neighbors at the current level and possibly at an upper pyramid level.
 * 
 * @param G Pointer to a 1D array of Image layers.
 * @param L The level of the image to build the neighborhood for.
 * @param x The x-coordinate of the pixel at level L.
 * @param y The y-coordinate of the pixel at level L.
 * 
 * @return Pointer to an array of RGB pixel values representing the neighborhood of pixel (x, y).
 *         Includes values from both the current and upper levels (if applicable).
 */
double *build_neighborhood(Image *G, int L, int x, int y) {
    // Number of coordinate pairs of neighborhood arrays
    int neigh_count = sizeof(neighborhood) / sizeof(neighborhood[0]);
    int upper_neigh_count = sizeof(upper_neighborhood) / sizeof(upper_neighborhood[0]);

    int pixel_count = (neigh_count + upper_neigh_count) * 3;
        
    double *pixels = malloc(pixel_count * sizeof(double));

    int idx = 0;

    // Current level neighborhood
    for (int i = 0; i < neigh_count; i++) {
        int dx = neighborhood[i][0]; // Horizontal offset
        int dy = neighborhood[i][1]; // Vertical offset

        // Toroidal coordinates (wrap-around bound)
        int x_i = (x + dx + G[L].width) % G[L].width;
        int y_i = (y + dy + G[L].height) % G[L].height;

        // 2d coord to 1d index
        int index = (x_i + G[L].width * y_i) * 3;

        // Append RGB to pixels array
        pixels[idx++] = G[L].data[index];     // Red
        pixels[idx++] = G[L].data[index + 1]; // Green
        pixels[idx++] = G[L].data[index + 2]; // Blue
    }

    // Upper level neighborhood
    if (L < MAX_LEVELS - 1) {
        for (int i = 0; i < upper_neigh_count; i++) {
            int dx = upper_neighborhood[i][0]; // Horizontal offset
            int dy = upper_neighborhood[i][1]; // Vertical offset

            // Downsample by factor of 2 to convert to parent level
            int x_i = (x / 2 + dx + G[L + 1].width) % G[L + 1].width;
            int y_i = (y / 2 + dy + G[L + 1].height) % G[L + 1].height;

            // 1d index to RGB
            int index = (x_i + G[L + 1].width * y_i) * 3;

            // Append RGB (from parent L) to pixels array
            pixels[idx++] = G[L + 1].data[index];     // Red
            pixels[idx++] = G[L + 1].data[index + 1]; // Green
            pixels[idx++] = G[L + 1].data[index + 2]; // Blue
        }
    }

    return pixels;
}


int match_neighborhood(double *Na, double *Ns, int l) {
    int res = 0;
    for (int i = 0; i < l; i++) {
        res += (Na[i] - Ns[i]) * (Na[i] - Ns[i]);
    }    
    return res;
}
