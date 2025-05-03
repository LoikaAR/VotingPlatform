#ifndef GAUSS_PYRAMID_H
#define GAUSS_PYRAMID_H

#include "image.h"

#define NUM_LEVELS 4  // lvls of the gauss pyramid (including original)

Image subsample(Image* src);

void im_gauss_filt(Image* src, Image* dest, int level, int total_levels);

Image* build_gauss_pyramid(Image img, int levels);

#endif