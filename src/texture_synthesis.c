#include "texture_synthesis.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#include "find_best_match.h"
#include "gauss_pyramid.h"
#include "image.h"

Image texture_synthesis(Image sample_img, int output_size, int levels) {
    /* 1. Initialize output image with random noise. */
    srand(0);  // Set seed for reproducibility
    int out_data_size = sample_img.channels * output_size * output_size;

    // Initialize output image
    Image output_img;
    output_img.width = output_size;
    output_img.height = output_size;  // assuming image is a square
    output_img.maxval = sample_img.maxval;
    output_img.channels = sample_img.channels;
    output_img.data = (int *)malloc(out_data_size * sizeof(int));

    // Fill with random noise
    for (int i = 0; i < output_size; i++) {
        for (int j = 0; j < output_size; j++) {
            for (int c = 0; c < 3; c++) {
                int idx = (i * output_size * output_img.channels) +
                          (j * output_img.channels) + c;
                output_img.data[idx] = rand() % 256;  // TODO: Use clamp maxval
            }
        }
    }

    save_ppm_image(output_img, "./img/out_origin.ppm");

    /* 2. Build Gaussian pytamids for sample and output. */

    printf("(%dx%d) -> (%dx%d)\n", sample_img.width, sample_img.height,
           output_img.width, output_img.height);

    printf("Building output_pyramid...\n");
    Image *output_pyr = build_gauss_pyramid(output_img, levels);
    printf("\t\tok\n");

    printf("Building sample pyramid...\n");
    Image *sample_pyr = build_gauss_pyramid(sample_img, levels);
    printf("\t\tok\n");

    /* Step 3: Texture synthesis by matching at each level. */
    printf("Synthesizing...\n");
    for (int l = levels - 1; l >= 0; l--) {
        // Needed for process logging
        int total_pixels = output_pyr[l].width * output_pyr[l].height;
        int current_pixel = 0;
        printf("  [Level 1/%d]:   0%%", levels);

        for (int y = 0; y < output_pyr[l].height; y++) {
            for (int x = 0; x < output_pyr[l].width; x++) {
                int best_match_idx =
                    find_best_match(sample_pyr, output_pyr, l, levels, y, x);

                int output_base = (y * output_pyr[l].width + x) * 3;
                int sample_base = best_match_idx * 3;

                for (int c = 0; c < 3; c++) {
                    output_pyr[l].data[output_base + c] =
                        sample_pyr[l].data[sample_base + c];
                }

                // Show progress
                if (++current_pixel % 100 == 0 ||
                    current_pixel == total_pixels) {
                    int percent = (current_pixel * 100) / total_pixels;
                    printf("\r%-50s", " ");
                    printf("\r  [Level %d/%d] Progress: %3d%%", levels - l,
                           levels, percent);
                    fflush(stdout);
                }
            }
        }

        char filename[64];
        sprintf(filename, "./img/out_level_%d.ppm", l);
        save_ppm_image(output_pyr[l], filename);
    }

    printf("\t\tok\n");

    return output_pyr[0];
}