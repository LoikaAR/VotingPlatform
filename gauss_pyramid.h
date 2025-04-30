#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>

#define NUM_LEVELS 4 // lvls of the gauss pyramid (including original)

struct Image subsample(int height, int width, int colors, struct Image pixels_in, struct Image pixels_out);

struct Image im_gauss_filt(double** kernel, int height, int width, int colors, int kr, struct Image pixels_in, struct Image pixels_out);

struct Image* build_gauss_pyramid(struct Image img);