#include "gauss_pyramid.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "image.h"

// take every 2nd pixel
Image subsample(Image* src) {
    int w2 = src->width / 2;
    int h2 = src->height / 2;

    Image dest = alloc_image(w2, h2, src->channels, src->maxval);

    for (int y = 0; y < h2; y++) {
        for (int x = 0; x < w2; x++) {
            for (int c = 0; c < src->channels; c++) {
                dest.data[(y * w2 + x) * src->channels + c] =
                    src->data[((2 * y) * src->width + (2 * x)) * src->channels +
                              c];
            }
        }
    }

    return dest;
}

#define KERNEL_RADIUS 2
static const double GAUSS_KERNEL[5][5] = {
    {1 / 256.0, 4 / 256.0, 6 / 256.0, 4 / 256.0, 1 / 256.0},
    {4 / 256.0, 16 / 256.0, 24 / 256.0, 16 / 256.0, 4 / 256.0},
    {6 / 256.0, 24 / 256.0, 36 / 256.0, 24 / 256.0, 6 / 256.0},
    {4 / 256.0, 16 / 256.0, 24 / 256.0, 16 / 256.0, 4 / 256.0},
    {1 / 256.0, 4 / 256.0, 6 / 256.0, 4 / 256.0, 1 / 256.0}};

// apply gauss kernel to an image
void gaussian_filter(Image* src, Image* dest, int level, int total_levels) {
    for (int y = 0; y < src->height; y++) {
        // Show progress
        float percent = (100.0 * (y + 1)) / src->height;
        printf("\r%-50s", " ");
        printf("\r  [Level %d/%d] Progress: %.2f%%", level + 1, total_levels,
               percent);
        fflush(stdout);

        for (int x = 0; x < src->width; x++) {
            for (int c = 0; c < src->channels; c++) {
                double sum = 0.0;

                for (int i = -KERNEL_RADIUS; i <= KERNEL_RADIUS; i++) {
                    int yi = y + i;
                    if (yi < 0) yi = -yi;
                    if (yi >= src->height) yi = 2 * src->height - yi - 1;

                    for (int j = -KERNEL_RADIUS; j <= KERNEL_RADIUS; j++) {
                        int xj = x + j;
                        if (xj < 0) xj = -xj;
                        if (xj >= src->width) xj = 2 * src->width - xj - 1;

                        double v =
                            (double)src
                                ->data[(yi * src->width + xj) * src->channels +
                                       c];
                        sum += v * GAUSS_KERNEL[i + 2][j + 2];
                    }
                }

                int val = (int)round(sum);
                if (val < 0) val = 0;
                if (val > src->maxval) val = src->maxval;
                dest->data[(y * src->width + x) * src->channels + c] = val;
            }
        }
    }
}

/**
 * @brief Builds neighborhood of pixel (x, y) at given level in a Gaussian
 * pyramid.
 *
 * @param img The image, starting level 0.
 *
 * @return The Gaussian pyramid derived from `img`.
 */
Image* build_gauss_pyramid(Image img, int levels) {
    Image* pyr = (Image*)malloc(levels * sizeof(Image));

    // Copy original image to level 0
    pyr[0] = alloc_image(img.width, img.height, img.channels, img.maxval);
    memcpy(pyr[0].data, img.data,
           img.width * img.height * img.channels * sizeof(int));

    // Build subsequent levels
    Image tmp;
    for (int l = 1; l < levels; l++) {
        tmp = alloc_image(pyr[l - 1].width, pyr[l - 1].height,
                          pyr[l - 1].channels, pyr[l - 1].maxval);

        // apply gauss filter and subsample
        gaussian_filter(&pyr[l - 1], &tmp, l, levels);
        pyr[l] = subsample(&tmp);

        char filename[64];
        sprintf(filename, "./img/pyr_level_%d.ppm", l);
        save_ppm_image(pyr[l], filename);

        free(tmp.data);
    }

    return pyr;
}
