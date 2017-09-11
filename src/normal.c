//
// Created by pierfied on 9/10/17.
//

#include <stdlib.h>
#include <math.h>
#include "normal.h"

/*
 * normal
 * Draws from a standard normal distribution using the
 * Box-Muller transformation.
 *
 * Returns:
 * Random realization from a standard normal distribution.
 */
double normal() {
    double x1,x2,w;
    do{
        x1 = 2.0 *rand() / RAND_MAX - 1.0;
        x2 = 2.0 * rand() / RAND_MAX - 1.0;

        w = x1*x1 + x2*x2;
    }while (w >= 1.0 || w == 0);

    double y0 = sqrt(-2.0 * log(w) / w);
    return x1 * y0;
}