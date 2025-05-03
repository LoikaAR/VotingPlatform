#ifndef NEIGHBORHOOD_H
#define NEIGHBORHOOD_H

#include "image.h"

int *build_neighborhood(Image *G, int L, int total_levels, int x, int y);

int match_neighborhood(int *Na, int *Ns, int l);

#endif