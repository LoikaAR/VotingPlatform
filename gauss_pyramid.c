#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include "gauss_filter.h"
#include "image_struct.h"

#define NUM_LEVELS 4 // lvls of the gauss pyramid (including original)

// take every 2nd pixel
struct Image subsample(int height, int width, int colors, 
    struct Image pixels_in, struct Image pixels_out) {
        int out_h = height/2, out_w = width/2;
    for (int i = 0; i < height/2; ++i) {
        for (int j = 0; j < width/2; ++j) {
            for (int k = 0; k < colors; ++k) {
                // take every 2nd pixel (2*i, 2*j)
                pixels_out.pixels[(i*width*colors) + (j*colors) + k] = pixels_in.pixels[(i*2*width*colors) + (j*2*colors) + k];
            }
        }
    }
    return pixels_out;
}

// apply gauss kernel to an image
struct Image im_gauss_filt(double** kernel, int height, int width, int colors, int kr, struct Image pixels_in, struct Image pixels_out) {
    for (int i = 0; i < height; i++) {
        printf("row %d\n", i);
        for (int j = 0; j < width; j++) {
            for (int k = 0; k < 3; k++) {
                double sum = 0.0;
                for (int ik = -kr; ik <= kr; ik++) {
                    for (int jk = -kr; jk <= kr; jk++) {
                        int ni = i + ik;
                        int nj = j + jk;
                        // apply kernel as a convolution op, if within the image
                        if (ni >= 0 && ni < height && nj >= 0 && nj < width) { // zero-padded
                            sum += kernel[ik + kr][jk + kr] * pixels_in.pixels[(ni*width*colors) + (nj*colors) + k];
                        }
                    }
                }
                pixels_out.pixels[(i*width*colors) + (j*colors) + k] = (int)sum;
            }
        }
    }
    return pixels_out;
}


int main() {
    struct Image img;

    FILE* fp = fopen("./img/cameraman_p3.ppm", "r");
    char header[3];
    int width, height, max_color;
    fscanf(fp,"%s", header);             // ppm header, rn works for P3 only
    fscanf(fp, "%d %d", &img.x, &img.y); // x = width, y = height
    fscanf(fp, "%d", &img.rgb_clamp);    // max rgb value
    img.L = 3;                           // number of color channels, does not change

    // check header format for color images
    if (header[0] != 'P' || header[1] != '3') {
        printf("Invalid file format.\n");
        printf("Header = %s\n", header);
        return 1;
    }

    img.pixels = (int*)malloc(img.x*img.y*img.L*sizeof(int));
    int pixels[height][width][3];
    // int ***out = (int***)malloc(height*width*3*sizeof(int));

    // place image into img struct
    for (int i = 0; i < img.y; i++) {
        for (int j = 0; j < img.x; j++) {
            for (int k = 0; k < img.L; k++) {
                fscanf(fp, "%d", &(img.pixels[(i*img.x*img.L) + (j*img.L) + k]));
            }
        }
    }
    fclose(fp); // close input file


    // at this point the image is only in pixels array
    double** kernel = make_gauss_kernel();
    int kr = kernel_size/2;
    struct Image gauss_pyramid[NUM_LEVELS]; 

    // 0th level of pyramid = original image
    gauss_pyramid[0].pixels = (int*)malloc(img.x*img.y*img.L*sizeof(int)); 
    memcpy(gauss_pyramid[0].pixels, img.pixels, img.x*img.y*img.L*sizeof(int));
    gauss_pyramid[0].x = img.x;
    gauss_pyramid[0].y = img.y;
    gauss_pyramid[0].L = img.L;
    gauss_pyramid[0].rgb_clamp = img.rgb_clamp;

    // build pyramid
    int cur_width, cur_height, prev_height, prev_width;
    for (int p = 1; p < NUM_LEVELS; p++) {
        printf("Building pyramid level %d\n", p);
        // instantiate struct
        gauss_pyramid[p].x = gauss_pyramid[p-1].x/2;    
        gauss_pyramid[p].y = gauss_pyramid[p-1].y/2;
        gauss_pyramid[p].L = gauss_pyramid[p-1].L;
        gauss_pyramid[p].rgb_clamp = gauss_pyramid[p-1].rgb_clamp;
        gauss_pyramid[p].pixels = (int*)malloc(img.x*img.y*img.L*sizeof(int));

        // apply gauss filter and subsample
        gauss_pyramid[p] = im_gauss_filt(kernel, img.y, img.x, img.L, kr, gauss_pyramid[p-1], gauss_pyramid[p]);
        gauss_pyramid[p] = subsample(img.y, img.x, img.L, gauss_pyramid[p], gauss_pyramid[p]);
    
        // save into new file
        char *filename;
        char s1[3];
        sprintf(s1, "%d", p);
        char *s0 = "./img/pyramid_lvl_";
        char *s2 = "_file.ppm";

        filename = malloc(strlen(s0) + strlen(s1) + strlen(s2) + 1);
        filename[0] = '\0';  // make it a string in mem
        strcat(filename, s0);
        strcat(filename, s1);
        strcat(filename, s2);

        printf("%s\n", filename);
        if (remove(filename) == 0) {
            printf("Previous out_file deleted\n");
        }

        FILE *out_f = fopen(filename, "w");
        fprintf(out_f, "P3\n");
        fprintf(out_f, "%d %d\n", cur_width, cur_height);
        fprintf(out_f, "%d\n", gauss_pyramid[p].rgb_clamp);

        for (int i = 0; i < cur_height; i++) {
            for (int j = 0; j < cur_width; j++) {
                for (int k = 0; k < 3; k++) {
                    fprintf(out_f, "%d ", gauss_pyramid[p].pixels[(i*img.x*img.L) + (j*img.L) + k]); // can replace img.L
                }
            }
            fprintf(out_f, "\n");
        }
        fclose(out_f); // close file
    }
}