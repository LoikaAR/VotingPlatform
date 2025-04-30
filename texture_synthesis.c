#include <stdio.h>
#include "texture_synthesis.h"
#include "gauss_filter.h"
#include "image_struct.h"
#include "find_best_match.h"
#include "gauss_pyramid.h"

int *texture_synthesis(struct Image Ia, int output_size, int pyramid_levels) {
    srand(0);
    int arraySize = 3 * output_size * output_size;
    // initialize image with random noise
    struct Image Is;
    Is.x = output_size;
    Is.y = output_size;
    Is.rgb_clamp = Ia.rgb_clamp;
    Is.pixels = (int*)malloc(arraySize * sizeof(int));

    for (int i = 0; i < arraySize; i++) {
        Is.pixels[i] = rand() % 255;
    }
    // create the gaussian pyramids
    struct Image *Gs = build_gauss_pyramid(Is); // Is
    struct Image *Ga = build_gauss_pyramid(Ia); // Ia

    for (int l = pyramid_levels - 1; l >= 0; l--) {
        for (int i = 0; i < Gs[l].x; i++) {
            for (int j = 0; j < Gs[l].y; j++) {
                int best_match = find_best_match(Ga, Gs, l, i, j);
                Gs[l].pixels[i * Gs[l].x + j] = Ga[l].pixels[best_match];
            }
        }
    }
    return Gs[0].pixels;

}

int main() {}