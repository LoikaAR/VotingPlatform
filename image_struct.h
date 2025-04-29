#include <stdio.h>

struct Image {
    int x;          // width
    int y;          // height
    int L;          // number of levels
    int rgb_clamp;  // max value of each color channel
    int *pixels;    // image pixels
};