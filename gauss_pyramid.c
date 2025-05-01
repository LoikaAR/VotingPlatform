#include "gauss_pyramid.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "gauss_filter.h"
#include "image.h"

// take every 2nd pixel
Image subsample(int height, int width, int colors, Image pixels_in,
                Image pixels_out) {
    int out_h = height / 2, out_w = width / 2;
    for (int i = 0; i < height / 2; ++i) {
        for (int j = 0; j < width / 2; ++j) {
            for (int k = 0; k < colors; ++k) {
                // take every 2nd pixel (2*i, 2*j)
                pixels_out.data[(i * width * colors) + (j * colors) + k] =
                    pixels_in
                        .data[(i * 2 * width * colors) + (j * 2 * colors) + k];
            }
        }
    }
    return pixels_out;
}

// apply gauss kernel to an image
Image im_gauss_filt(double** kernel, int height, int width, int colors, int kr,
                    Image pixels_in, Image pixels_out) {
    for (int i = 0; i < height; i++) {
        printf("row %d\n", i);
        for (int j = 0; j < width; j++) {
            for (int k = 0; k < colors; k++) {
                double sum = 0.0;
                for (int ik = -kr; ik <= kr; ik++) {
                    for (int jk = -kr; jk <= kr; jk++) {
                        int ni = i + ik;
                        int nj = j + jk;
                        // apply kernel as a convolution op, if within the image
                        if (ni >= 0 && ni < height && nj >= 0 &&
                            nj < width) {  // zero-padded
                            sum += kernel[ik + kr][jk + kr] *
                                   pixels_in.data[(ni * width * colors) +
                                                  (nj * colors) + k];
                        }
                    }
                }
                pixels_out.data[(i * width * colors) + (j * colors) + k] =
                    (int)sum;
            }
            // printf("here?\n");
        }
    }
    return pixels_out;
}

Image* build_gauss_pyramid(Image img) {
    double** kernel = make_gauss_kernel();
    int kr = kernel_size / 2;
    // struct Image gauss_pyramid[NUM_LEVELS];
    Image* gauss_pyramid = malloc(NUM_LEVELS * sizeof(Image));

    // 0th level of pyramid = original image
    gauss_pyramid[0].data =
        (int*)malloc(img.width * img.height * img.channels * sizeof(int));
    memcpy(gauss_pyramid[0].data, img.data,
           img.width * img.height * img.channels * sizeof(int));

    gauss_pyramid[0].width = img.width;
    gauss_pyramid[0].height = img.height;
    gauss_pyramid[0].channels = img.channels;
    gauss_pyramid[0].maxval = img.maxval;

    // build pyramid
    int cur_width, cur_height, prev_height, prev_width;
    for (int p = 1; p < NUM_LEVELS; p++) {
        printf("Building pyramid level %d\n", p);
        // instantiate struct
        gauss_pyramid[p].width = gauss_pyramid[p - 1].width / 2;
        gauss_pyramid[p].height = gauss_pyramid[p - 1].height / 2;
        gauss_pyramid[p].channels = gauss_pyramid[p - 1].channels;
        gauss_pyramid[p].maxval = gauss_pyramid[p - 1].maxval;
        gauss_pyramid[p].data =
            (int*)malloc(img.width * img.height * img.channels * sizeof(int));

        prev_height = gauss_pyramid[p - 1].height;
        prev_width = gauss_pyramid[p - 1].width;
        cur_height = gauss_pyramid[p].height;
        cur_width = gauss_pyramid[p].width;

        // apply gauss filter and subsample
        // gauss_pyramid[p] = im_gauss_filt(kernel, img.height, img.width,
        // img.channels, kr, gauss_pyramid[p-1], gauss_pyramid[p]);
        gauss_pyramid[p] = subsample(img.height, img.width, img.channels,
                                     gauss_pyramid[p - 1], gauss_pyramid[p]);

        // save into new file
        char* filename;
        char s1[3];
        sprintf(s1, "%d", p);
        // char* id = &img;
        char* s0 = "./img/pyramid_lvl_";
        char* s2 = "_file.ppm";

        filename = malloc(strlen(s0) + strlen(s1) + strlen(s2) + 1);
        filename[0] = '\0';  // make it a string in mem
        strcat(filename, s0);
        strcat(filename, s1);
        strcat(filename, s2);

        printf("%s\n", filename);
        if (remove(filename) == 0) {
            // printf("Previous out_file deleted\n");
        }

        FILE* out_f = fopen(filename, "w");
        fprintf(out_f, "P3\n");
        fprintf(out_f, "%d %d\n", cur_width, cur_height);
        fprintf(out_f, "%d\n", gauss_pyramid[p].maxval);

        for (int i = 0; i < cur_height; i++) {
            for (int j = 0; j < cur_width; j++) {
                for (int k = 0; k < 3; k++) {
                    fprintf(
                        out_f, "%d ",
                        gauss_pyramid[p].data[(i * img.width * img.channels) +
                                              (j * img.channels) +
                                              k]);  // can replace img.L
                }
            }
            fprintf(out_f, "\n");
        }
        fclose(out_f);  // close file
    }
    return gauss_pyramid;
}

// int _main() {
// struct Image img;
// char* image_path = "./img/texture_1.ppm";
// FILE* fp = fopen(image_path, "r");
// char header[3];
// int width, height, max_color;
// fscanf(fp, "%s", header);  // ppm header, rn works for P3 only
// fscanf(fp, "%d %d", &img.width, &img.height);  // x = width, y = height
// fscanf(fp, "%d", &img.maxval);                 // max rgb value
// img.channels = 3;  // number of color channels, does not change

// // check header format for color images
// if (header[0] != 'P' || header[1] != '3') {
//     printf("Invalid file format.\n");
//     printf("Header = %s\n", header);
//     return 1;
// }

// img.data = (int*)malloc(img.width * img.height * img.channels * sizeof(int));

// // place image into img struct
// for (int i = 0; i < img.height; i++) {
//     for (int j = 0; j < img.width; j++) {
//         for (int k = 0; k < img.channels; k++) {
//             fscanf(fp, "%d",
//                    &(img.data[(i * img.width * img.channels) +
//                               (j * img.channels) + k]));
//         }
//     }
// }
// fclose(fp);  // close input file

// struct Image* gauss_pyramid = build_gauss_pyramid(img);
// }