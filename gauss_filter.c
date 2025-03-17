#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "gauss_filter.h"

#define kernel_size 5
#define sigma 1.0
#define PI 3.1415926535

double** make_gauss_kernel() {
    double *data = (double*)malloc(kernel_size*kernel_size*sizeof(double));
    double **kernel = (double**)malloc(kernel_size*sizeof(double*));

    double sum = 0.0;
    int i, j;

    for (int k = 0; k < kernel_size; k++) {
        kernel[k] = data+k*kernel_size;
    }

    // set up kernel
    for (i = 0; i < kernel_size; i++) {
        for (j = 0; j < kernel_size; j++) {
            // center i and j about center
            double x = i - (kernel_size-1)/2.0;
            double y = j - (kernel_size-1)/2.0;
            
            kernel[i][j] = exp(-((x*x)+(y*y)) / (2*(sigma*sigma)));
            kernel[i][j] /= (2*PI*(sigma*sigma));
        }
    }
    return kernel;
}

// int main() {
//     double **k = make_gauss_kernel();
//     for (int i = 0; i < kernel_size; i++) {
//         for (int j = 0; j < kernel_size; j++) {
//             printf("%f ", k[i][j]);
//         }
//         printf("\n");
//     }
// }