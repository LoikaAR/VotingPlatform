#include <stdio.h>
#include "texture_synthesis.h"
#include "gauss_filter.h"
#include "image.h"
#include "find_best_match.h"
#include "gauss_pyramid.h"

Image texture_synthesis(Image Ia, int output_size, int pyramid_levels) {
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
    
    printf("we here\n");

    for (int l = pyramid_levels - 1; l >= 0; l--) {
        for (int i = 0; i < Gs[l].width; i++) {
            for (int j = 0; j < Gs[l].height; j++) {
                int best_match = find_best_match(Ga, Gs, l, i, j);
                int base_s = (i * Gs[l].width + j) * 3;
                int base_a = best_match * 3;
                Gs[l].data[base_s]     = Ga[l].data[base_a];
                Gs[l].data[base_s + 1] = Ga[l].data[base_a + 1];
                Gs[l].data[base_s + 2] = Ga[l].data[base_a + 2];
            }
        }
    }
    return Gs[0];

}

int main() {
    struct Image img;
    char* image_path = "./img/field_p3.ppm";
    FILE* fp = fopen(image_path, "r");
    if (fp == NULL) {
        perror("Error opening file");
        return 1;
    }
    char header[3];
    int width, height, max_color;
    fscanf(fp,"%s", header);             // ppm header, rn works for P3 only
    fscanf(fp, "%d %d", &img.width, &img.height); // x = width, y = height
    fscanf(fp, "%d", &img.maxval);    // max rgb value
    img.channels = 3;                           // number of color channels, does not change
    // check header format for color images
    if (header[0] != 'P' || header[1] != '3') {
        printf("Invalid file format.\n");
        printf("Header = %s\n", header);
        return 1;
    }
    
    img.data = (double*)malloc(img.width*img.height*img.channels*sizeof(double));
    // place image into img struct
    for (int i = 0; i < img.height; i++) {
        for (int j = 0; j < img.width; j++) {
            for (int k = 0; k < img.channels; k++) {
                fscanf(fp, "%lf", &(img.data[(i*img.width*img.channels) + (j*img.channels) + k]));
            }
        }
    }
    fclose(fp); // close input file

    Image Gs = texture_synthesis(img, 512, 4);
    double *output = Gs.data;


    // save as ppm
    // Save the output as a PPM file
    FILE* out_fp = fopen("output.ppm", "w");
    if (out_fp == NULL) {
        printf("Error opening output file.\n");
        return 1;
    }

    // Write the PPM header
    fprintf(out_fp, "P3\n");
    fprintf(out_fp, "%d %d\n", 512, 512); // New image dimensions
    fprintf(out_fp, "255\n"); // Max color value (assuming 8-bit colors)

    // Write pixel data (output array should be in RGB format)
    for (int i = 0; i < 512; i++) { // Output image height
        for (int j = 0; j < 512; j++) { // Output image width
            int idx = (i * 512 * 3) + (j * 3); // Index for RGB values in the flat array
            fprintf(out_fp, "%d %d %d ", 
                    (int)(output[idx]), 
                    (int)(output[idx + 1]), 
                    (int)(output[idx + 2]));
        }
        fprintf(out_fp, "\n");
    }

    fclose(out_fp); // Close the output file

    free(img.data); // Free input image memory
    free(output);   // Free synthesized image memory

    return 0;
    
}