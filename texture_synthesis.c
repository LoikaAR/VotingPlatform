#include <stdio.h>
#include "texture_synthesis.h"
#include "gauss_filter.h"
#include "image_struct.h"

int *texture_synthesis(int *Ia, int outputSize, int pyramidLevels) {
    // initialize image with random noise
    srand(0);
    int arraySize = 3* outputSize * outputSize;
    int *Is = (int*)malloc(arraySize * sizeof(int));
    int i;
    for (i = 0; i < arraySize; i++) {
        Is[i] = rand() % 255;
    }
    // create the gaussian pyramids
    struct Image *Gs;
    struct Image *Ga;

    for (int i = pyramidLevels; i >= 0; i--) {
        for () {
            for () {
                
            }
        }

    }
}

int main() {}