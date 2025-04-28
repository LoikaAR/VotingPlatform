#include <stdio.h>

struct Image {
    int x; // width
    int y; // heigth
    int L; // number of levels
    int rgb_clamp; // max value of each color channel
    double *Image; // array containing all the double images
};