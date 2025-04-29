#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include "gauss_filter.h"
#include "image_struct.h"

#define NUM_LEVELS 2 // lvls of the gauss pyramid (including original)


void zero_out(int height, int width, int colors, int in[height][width][colors]) {
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            for (int k = 0; k < 3; k++) {
                in[i][j][k] = 0;
            }
        }
    }
}

// take every 2nd pixel
void subsample(int height, int width, int colors, 
    struct Image pixels_in, struct Image pixels_out) {
        int out_h = height/2, out_w = width/2;
    for (int i = 0; i < out_h; ++i) {
        for (int j = 0; j < out_w; ++j) {
            for (int k = 0; k < colors; ++k) {
                pixels_out.pixels[(i*width*colors) + (j*colors) + k] = pixels_in.pixels[(i*2*width*colors) + (j*2*colors) + k];
            }
        }
    }
}

// apply gauss filter
void im_gauss_filt(double** kernel, int height, int width, int colors, int kr, struct Image pixels_in, struct Image pixels_out) {
    for (int i = 0; i < height; i++) {
        printf("row %d\n", i);
        for (int j = 0; j < width; j++) {
            for (int k = 0; k < 3; k++) {
                
                double sum = 0.0;
                for (int ik = -kr; ik <= kr; ik++) {
                    for (int jk = -kr; jk <= kr; jk++) {
                        int ni = i + ik;
                        int nj = j + jk;

                        if (ni >= 0 && ni < height && nj >= 0 && nj < width) { // zero-padded
                            sum += kernel[ik + kr][jk + kr] * pixels_in.pixels[(ni*width*colors) + (nj*colors) + k];
                        }
                    }
                }
                pixels_out.pixels[(i*width*colors) + (j*colors) + k] = (int)sum;
                // [i][j][k] = (int)sum;
            }
        }
    }
}

void cpy_img_gp(int height, int width, int colors, struct Image src, struct Image dest) {
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            for (int c = 0; c < 3; ++c) {
                dest.pixels[(i*dest.x*dest.L) + (j*dest.L) + c] = src.pixels[(i*src.x*src.L) + (j*src.L) + c];
            }
        }
    }
}



int main() {
    // delete out_file if it exists
    const char *file = "./img/out_file.ppm";

    if (remove(file) == 0) {
        printf("Previous out_file deleted\n");
    }

    struct Image img;

    FILE* fp = fopen("./img/cameraman_p3.ppm", "r");
    char header[3];
    int width, height, max_color;
    fscanf(fp,"%s", header);
    fscanf(fp, "%d %d", &img.x, &img.y); // x = width, y = height
    fscanf(fp, "%d", &img.rgb_clamp);
    img.L = 3;
    // int colors = 3;

    // check header format
    // for color images
    if (header[0] != 'P' || header[1] != '3') {
        printf("Invalid file format.\n");
        printf("Header = %s\n", header);
        return 1;
    }

    img.pixels = (int*)malloc(img.x*img.y*img.L*sizeof(int));

    int pixels[height][width][3];
    // int ***out = (int***)malloc(height*width*3*sizeof(int));

    for (int i = 0; i < img.y; i++) {
        for (int j = 0; j < img.x; j++) {
            for (int k = 0; k < img.L; k++) {
                fscanf(fp, "%d", &(img.pixels[(i*img.x*img.L) + (j*img.L) + k]));
            }
        }
    }
    fclose(fp); // close input file


    // at this point the image is only in pixels array
    // apply gauss filter
    double** kernel = make_gauss_kernel();
    int kr = kernel_size/2;

    struct Image gauss_pyramid[NUM_LEVELS]; 

    gauss_pyramid[0].pixels = (int*)malloc(img.x*img.y*img.L*sizeof(int)); 
    // gauss_pyramid[1].pixels = (int*)malloc(img.x*img.y*img.L*sizeof(int)); 
    
    memcpy(gauss_pyramid[0].pixels, img.pixels, img.x*img.y*img.L*sizeof(int));

    // build pyramid 
    int cur_height = height, cur_width = width;
    for (int p = 1; p < NUM_LEVELS; p++) {
        printf("Building pyramid level %d\n", p);
        gauss_pyramid[p].x = gauss_pyramid[p-1].x/2;
        gauss_pyramid[p].y = gauss_pyramid[p-1].y/2;
        gauss_pyramid[p].L = gauss_pyramid[p-1].L;
        gauss_pyramid[p].pixels = (int*)malloc(img.x*img.y*img.L*sizeof(int));

        // im_gauss_filt(kernel, height, width, colors, kr, gauss_pyramid[p-1], gauss_pyramid[p]);
        subsample(gauss_pyramid[p].y, gauss_pyramid[p].x, img.L, gauss_pyramid[p-1], gauss_pyramid[p]);
    
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
        fprintf(out_f, "%d\n", max_color);

        for (int i = 0; i < cur_height; i++) {
            for (int j = 0; j < cur_width; j++) {
                for (int k = 0; k < 3; k++) {
                    fprintf(out_f, "%d ", gauss_pyramid[p].pixels);
                    // printf("%d ", gauss_pyramid[i][j][k][p]);
                }
            }
            fprintf(out_f, "\n");
        }
        fclose(out_f); // close file
    }
}