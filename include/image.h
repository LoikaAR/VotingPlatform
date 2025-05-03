#ifndef IMAGE_H
#define IMAGE_H

// Structure to represent an image.
typedef struct Image {
    int width;     // Image width in px
    int height;    // Image height in px
    int channels;  // Number of color channels (e.g. 1 = grayscale, 3 = RGB)
    int maxval;    // Maximum value of pixel value
    int *data;     // Pointer to image data, flat array of RGB pixels
} Image;

Image load_ppm_image(const char *path);

void save_ppm_image(const Image img, const char *dest);

Image alloc_image(int width, int height, int channels, int maxval);

#endif