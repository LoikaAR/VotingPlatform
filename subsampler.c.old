#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// take every 2nd pixel
void subsample(int old_height, int old_width, int new_height, int new_width, int colors, 
    int in[old_height][old_width][colors], int out[new_height][new_width][colors]) {

    int width_m2 = old_width % 2 == 1 ? old_width - 1: old_width;
    for (int i = 0; i < new_height; i++) {
        for (int j = 0; j < new_width; j+=2) {
            for (int k = 0; k < colors; k++) {
                out[i][j][k] = in[i*2][j*2][k];
            }
        }
    }
}

int _main() {
    // delete out_file if it exists
    const char *file = "./img/sub_file.ppm";

    if (remove(file) == 0) {
        printf("Previous sub_file deleted\n");
    }
    FILE* fp = fopen("./img/cameraman_p3.ppm", "r");
    char header[3];
    int width, height, max_color; // max_color = #channels?
    fscanf(fp,"%s", header);
    fscanf(fp, "%d %d", &width, &height);
    fscanf(fp, "%d", &max_color);
    int colors = 3;

    // check header format
    // for color images
    if (header[0] != 'P' || header[1] != '3') {
        printf("Invalid file format.\n");
        printf("Header = %s\n", header);
        return 1;
    }

    int new_width = width/2;
    int new_height = height/2;
    int pixels[height][width][3];
    int out[new_height][new_width][3];
    // int ***out = (int***)malloc(height*width*3*sizeof(int));

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            for (int k = 0; k < 3; k++) {
                fscanf(fp, "%d", &pixels[i][j][k]);
                // out[i][j][k] = 0;
            }
        }
    }
    fclose(fp); // close input file

    subsample(height, width, new_height, new_width, colors, pixels, out);


     // save into new file
    FILE *out_f = fopen("./img/sub_file.ppm", "w");
    fprintf(out_f, "P3\n");
    fprintf(out_f, "%d %d\n", new_width, new_height);
    fprintf(out_f, "%d\n", max_color);

    for (int i = 0; i < new_height; i++) {
        for (int j = 0; j < new_width; j++) {
            for (int k = 0; k < 3; k++) {
                fprintf(out_f, "%d ", out[i][j][k]);
            }
        }
        fprintf(out_f, "\n");
    }

    fclose(out_f); // close file

}