#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "gauss_filter.h"

int main(){
    FILE* fp = fopen("./img/cameraman_p3.ppm", "r");
    char header[3];
    int width, height, max_color; // max_color = #channels?
    fscanf(fp,"%s", header);
    fscanf(fp, "%d %d", &width, &height);
    fscanf(fp, "%d", &max_color);

    // check header format
    // for color images
    if (header[0] != 'P' || header[1] != '3') {
        printf("Invalid file format.\n");
        printf("Header = %s\n", header);
        return 1;
    }

    int pixels[height][width][3];
    int out[height][width][3];


    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            for (int k = 0; k < 3; k++) {
                fscanf(fp, "%d", &pixels[i][j][k]);
                out[i][j][k] = 0;
            }
        }
    }

    fclose(fp); // close input file
    // at this point the image is in pixels array - very cool
    // apply gauss filter
    double** kernel = make_gauss_kernel();
    int kr = kernel_size/2;


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
                            sum += kernel[ik + kr][jk + kr] * pixels[ni][nj][k];
                        }
                    }
                }

                // printf("setting %d %d %d\n", i, j, k);
                out[i][j][k] = (int)sum;
            }
        }
    }


    // save into new file
    FILE *out_f = fopen("./img/out_file.ppm", "w");
    fprintf(out_f, "P3\n");
    fprintf(out_f, "%d %d\n", width, height);
    fprintf(out_f, "%d\n", max_color);

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            for (int k = 0; k < 3; k++) {
                fprintf(out_f, "%d ", out[i][j][k]);
            }
        }
        fprintf(out_f, "\n");
    }

    fclose(out_f); // close file
}