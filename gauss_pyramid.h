#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "image.h"

#define NUM_LEVELS 4  // lvls of the gauss pyramid (including original)

Image subsample(int height, int width, int colors, Image pixels_in,
                Image pixels_out);

Image im_gauss_filt(double** kernel, int height, int width, int colors, int kr,
                    Image pixels_in, Image pixels_out);

Image* build_gauss_pyramid(Image img);