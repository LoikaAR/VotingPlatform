#include "texture_synthesis.h"

#include <stdio.h>

#include "find_best_match.h"
#include "gauss_filter.h"
#include "gauss_pyramid.h"
#include "image.h"

Image texture_synthesis(Image sample_img, int output_size, int pyramid_levels) {
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

    printf("> sample_img (%d x %d)\n", sample_img.width, sample_img.height);
    printf("> output_img (%d x %d)\n", output_img.width, output_img.height);

    Image *output_pyr = build_gauss_pyramid(output_img);
    printf("> Built output_pyramid\n");
    Image *sample_pyr = build_gauss_pyramid(sample_img);
    printf("> Built sample_pyramid\n");

    /* Step 3: Texture synthesis by matching at each level. */
    for (int l = pyramid_levels - 1; l >= 0; l--) {
        printf("Synthesizing at pyramid level %d...\n", l);

        for (int y = 0; y < output_pyr[l].height; y++) {
            for (int x = 0; x < output_pyr[l].width; x++) {
                int best_match_idx =
                    find_best_match(sample_pyr, output_pyr, l, y, x);

                int output_base = (y * output_pyr[l].width + x) * 3;
                int sample_base = best_match_idx * 3;

                for (int c = 0; c < 3; c++) {
                    output_pyr[l].data[output_base + c] =
                        sample_pyr[l].data[sample_base + c];
                }
            }
        }

        char filename[64];
        sprintf(filename, "./img/out_level_%d.ppm", l);
        save_ppm_image(output_pyr[l], filename);
    }

    return output_pyr[0];
}

int main() {
    Image sample = load_ppm_image("./img/pebbles_64.ppm");

    Image Gs = texture_synthesis(sample, 128, 3);

    save_ppm_image(Gs, "./img/output.ppm");

    free(sample.data);  // Free input image memory
    free(Gs.data);      // Free synthesized image memory

    return 0;
}