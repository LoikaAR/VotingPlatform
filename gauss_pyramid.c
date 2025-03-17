#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "gauss_filter.h"

int main(){
    FILE* fp = fopen("./img/colorP3File.ppm", "r");
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

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            for (int k = 0; k < 3; k++) {
                fscanf(fp, "%d", &pixels[i][j][k]);
            }
        }
    }

    fclose(fp); // close input file

    // at this point the image is in pixels array - very cool
    // processing
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            for (int k = 0; k < 3; k++) {
                pixels[i][j][k] += 5;
            }
        }
    }


    double** kernel = make_gauss_kernel();


    // save into new file
    FILE *out_f = fopen("./img/out_file.ppm", "w");
    fprintf(out_f, "P3\n");
    fprintf(out_f, "%d %d\n", width, height);
    fprintf(out_f, "%d\n", max_color);

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            for (int k = 0; k < 3; k++) {
                fprintf(out_f, "%d ", pixels[i][j][k]);
            }
        }
        fprintf(out_f, "\n");
    }

    fclose(out_f); // close file
}