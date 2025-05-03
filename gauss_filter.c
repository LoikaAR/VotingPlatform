#include "gauss_filter.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

// IMPORTANT! Free both kernel[0] and kernel when not needed.
int **make_gauss_kernel() {
    int *data =
        (int *)calloc(kernel_size * kernel_size, sizeof(int));  // some 0s
    int **kernel = (double **)malloc(kernel_size * sizeof(int *));

    double sum = 0.0;
    int i, j;

    for (int k = 0; k < kernel_size; k++) {
        kernel[k] = data + k * kernel_size;
    }

    // set up kernel
    for (i = 0; i < kernel_size; i++) {
        for (j = 0; j < kernel_size; j++) {
            // center i and j about center
            double x = i - (kernel_size - 1) / 2.0;
            double y = j - (kernel_size - 1) / 2.0;

            kernel[i][j] = exp(-((x * x) + (y * y)) / (2 * (sigma * sigma)));
            kernel[i][j] /= (2 * PI * (sigma * sigma));
        }
    }
    return kernel;
}
