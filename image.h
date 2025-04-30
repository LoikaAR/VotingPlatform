#ifndef IMAGE_H
#define IMAGE_H

// Structure to represent an image
typedef struct {
    int width;    // Image width in px
    int height;   // Image height in px
    int channels; // Number of color channels (e.g. 1 = grayscale, 3 = RGB)
    int maxval;   // Maximum value of pixel value
    double *data; // Pointer to image data, flat array of RGB pixels
} Image;

#endif