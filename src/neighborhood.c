#include "neighborhood.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "image.h"

// TODO: Determine values
#define BASE_WIDTH 4
#define BASE_HEIGHT 4
#define MAX_LEVELS 4

#define NEIGHBORHOOD_SIZE 5
#define UPPER_NEIGHBORHOOD_SIZE 3

// Precompute the neighborhoods as arrays of relative coordinates

// Neighborhood for the current level (5x5)
// const int neighborhood[NEIGHBORHOOD_SIZE * NEIGHBORHOOD_SIZE][2] = {
//     {-2, -2}, {-2, -1}, {-2, 0}, {-2, 1}, {-2, 2},  // Row 1
//     {-1, -2}, {-1, -1}, {-1, 0}, {-1, 1}, {-1, 2},  // Row 2
//     {0, -2},  {0, -1},  {0, 0},  {0, 1},  {0, 2},   // Row 3
//     {1, -2},  {1, -1},  {1, 0},  {1, 1},  {1, 2},   // Row 4
//     {2, -2},  {2, -1},  {2, 0},  {2, 1},  {2, 2}    // Row 5
// };

// // Neighborhood for the next coarser level (3x3)
// const int upper_neighborhood[UPPER_NEIGHBORHOOD_SIZE *
// UPPER_NEIGHBORHOOD_SIZE]
//                             [2] = {
//                                 {-1, -1}, {-1, 0}, {-1, 1},  // Row 1
//                                 {0, -1},  {0, 0},  {0, 1},   // Row 2
//                                 {1, -1},  {1, 0},  {1, 1}    // Row 3
// };

// Neighborhood for the current level (5x5)
const int neighborhood[NEIGHBORHOOD_SIZE * NEIGHBORHOOD_SIZE][2] = {
    {-2, -2}, {-2, -1}, {-2, 0}, {-2, 1}, {-2, 2},  // Row 1
    {-1, -2}, {-1, -1}, {-1, 0}, {-1, 1}, {-1, 2},  // Row 2
    {0, -2},  {0, -1}};

// Neighborhood for the next coarser level (3x3)
const int upper_neighborhood[UPPER_NEIGHBORHOOD_SIZE * UPPER_NEIGHBORHOOD_SIZE]
                            [2] = {
                                {-1, -1}, {-1, 0}, {-1, 1},  // Row 1
                                {0, -1},  {0, 0},  {0, 1},   // Row 2
                                {1, -1},  {1, 0},  {1, 1}    // Row 3
};

/**
 * @brief Builds neighborhood of pixel (x, y) at given level in a
 * Gaussian pyramid.
 *
 * This function returns the neighborhood of a specific pixel at level L
 * in the pyramid, including the pixel's neighbors at the current level
 * and possibly at an upper pyramid level.
 *
 * @param G Pointer to a 1D array of Image layers.
 * @param L The level of the image to build the neighborhood for.
 * @param x The x-coordinate of the pixel at level L.
 * @param y The y-coordinate of the pixel at level L.
 *
 * @return Pointer to an array of RGB pixel values representing the
 * neighborhood of pixel (x, y). Includes values from both the current
 * and upper levels (if applicable).
 */
int *build_neighborhood(Image *G, int L, int total_levels, int x, int y) {
    // Number of coordinate pairs of neighborhood arrays
    int n0 = 12, n1 = (L < total_levels - 1 ? 9 : 0);
    // int neigh_count = sizeof(neighborhood) / sizeof(neighborhood[0]);
    // int upper_neigh_count =
    //     sizeof(upper_neighborhood) / sizeof(upper_neighborhood[0]);
    int pixel_count = (n0 + n1) * G->channels;
    int *pixels = (int *)malloc(pixel_count * sizeof(int));
    if (!pixels) return NULL;

    int idx = 0;

    // Current level neighborhood
    for (int i = 0; i < n0; i++) {
        int dx = neighborhood[i][0];  // Horizontal offset
        int dy = neighborhood[i][1];  // Vertical offset

        // Toroidal coordinates (wrap-around bound)
        int x_i = (x + dx + G[L].width) % G[L].width;
        int y_i = (y + dy + G[L].height) % G[L].height;

        // 2d coord to 1d index
        int index = (y_i + G[L].width * x_i) * G[L].channels;
        if (index < 0 ||
            index + 2 >= G[L].width * G[L].height * G[L].channels) {
            free(pixels);
            return NULL;
        }

        // Append RGB to pixels array
        pixels[idx++] = G[L].data[index];      // Red
        pixels[idx++] = G[L].data[index + 1];  // Green
        pixels[idx++] = G[L].data[index + 2];  // Blue
    }

    // Upper level neighborhood
    if (n1) {
        for (int i = 0; i < n1; i++) {
            int dx = upper_neighborhood[i][0];  // Horizontal offset
            int dy = upper_neighborhood[i][1];  // Vertical offset

            // Downsample by factor of 2 to convert to parent level
            int x_i = (x / 2 + dx + G[L + 1].width) % G[L + 1].width;
            int y_i = (y / 2 + dy + G[L + 1].height) % G[L + 1].height;

            // 1d index to RGB
            int index = (y_i + G[L + 1].width * x_i) * G[L + 1].channels;
            if (index < 0 ||
                index + 2 >= G[L].width * G[L].height * G[L].channels) {
                free(pixels);
                return NULL;
            }

            // Append RGB (from parent L) to pixels array
            pixels[idx++] = G[L + 1].data[index];      // Red
            pixels[idx++] = G[L + 1].data[index + 1];  // Green
            pixels[idx++] = G[L + 1].data[index + 2];  // Blue
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
