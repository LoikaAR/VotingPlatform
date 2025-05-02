#include "image.h"

#include <stdio.h>
#include <stdlib.h>

Image load_ppm_image(const char *path) {
    Image img;
    FILE *fp = fopen(path, "r");
    if (fp == NULL) {
        perror("Error opening file");
        exit(1);
    }

    char header[3];
    fscanf(fp, "%s", header);
    if (header[0] != 'P' || header[1] != '3') {
        printf("Invalid file format. Expected P3, got: %s\n", header);
        fclose(fp);
        exit(1);
    }

    fscanf(fp, "%d %d", &img.width, &img.height);
    fscanf(fp, "%d", &img.maxval);
    img.channels = 3;

    int data_size = img.width * img.height * img.channels;
    img.data = (int *)malloc(data_size * sizeof(int));
    if (!img.data) {
        perror("Memory allocation failed");
        fclose(fp);
        exit(1);
    }

    for (int i = 0; i < img.height; i++) {
        for (int j = 0; j < img.width; j++) {
            for (int k = 0; k < img.channels; k++) {
                fscanf(fp, "%d",
                       &img.data[(i * img.width * img.channels) +
                                 (j * img.channels) + k]);
            }
        }
    }

    fclose(fp);
    return img;
}

void save_ppm_image(const Image img, const char *dest) {
    FILE *fp = fopen(dest, "w");
    if (fp == NULL) {
        perror("Error opening output file");
        exit(1);
    }

    fprintf(fp, "P3\n");
    fprintf(fp, "%d %d\n", img.width, img.height);
    fprintf(fp, "%d\n", img.maxval);

    for (int i = 0; i < img.height; i++) {
        for (int j = 0; j < img.width; j++) {
            int idx = (i * img.width * img.channels) + (j * img.channels);
            fprintf(fp, "%d %d %d ", img.data[idx], img.data[idx + 1],
                    img.data[idx + 2]);
        }
        fprintf(fp, "\n");
    }

    fclose(fp);
}