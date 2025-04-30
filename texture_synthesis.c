#include <stdio.h>
#include "texture_synthesis.h"
#include "gauss_filter.h"
#include "image_struct.h"
#include "find_best_match.h"

int *texture_synthesis(int *Ia, int outputSize, int pyramidLevels) {
    // initialize image with random noise
    srand(0);
    int arraySize = 3 * outputSize * outputSize;
    int *Is = (int*)malloc(arraySize * sizeof(int));
    int i;
    for (i = 0; i < arraySize; i++) {
        Is[i] = rand() % 255;
    }
    // create the gaussian pyramids
    struct Image *Gs; // Is
    struct Image *Ga; // Ia

    for (int l = pyramidLevels - 1; l >= 0; l--) {
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