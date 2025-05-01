#include <stdio.h>
#include "texture_synthesis.h"
#include "gauss_filter.h"
#include "image.h"
#include "find_best_match.h"
#include "gauss_pyramid.h"

double *texture_synthesis(Image Ia, int output_size, int pyramid_levels) {
    srand(0);
    int arraySize = 3 * output_size * output_size;
    // initialize image with random noise
    Image Is;
    Is.width = output_size;
    Is.height = output_size;
    Is.maxval = Ia.maxval;
    Is.data = (double*)malloc(arraySize * sizeof(double));

    for (int i = 0; i < arraySize; i++) {
        Is.data[i] = rand() % 255;
    }
    // create the gaussian pyramids
    Image *Gs = build_gauss_pyramid(Is); // Is
    Image *Ga = build_gauss_pyramid(Ia); // Ia

    for (int l = pyramid_levels - 1; l >= 0; l--) {
        for (int i = 0; i < Gs[l].width; i++) {
            for (int j = 0; j < Gs[l].height; j++) {
                int best_match = find_best_match(Ga, Gs, l, i, j);
                Gs[l].data[i * Gs[l].width + j] = Ga[l].data[best_match];
            }
        }
    }
    return Gs[0].data;

}

int main() {}