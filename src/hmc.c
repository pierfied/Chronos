//
// Created by pierfied on 9/10/17.
//

#include <stdio.h>
#include <stdlib.h>
#include "hmc.h"
#include "normal.h"

/*
 * hmc
 * Generates a Markov chain of samples from the provided
 * likelihood distribution.
 *
 * Params:
 * hmc_args: HMCArgs struct containing the sampler arguments.
 *
 * Returns:
 * A SampleChain struct containing the full Markov chain.
 */
SampleChain hmc(HMCArgs hmc_args) {

}

double *initialize_momenta(int num_params) {
    double *p = malloc(sizeof(double) * num_params);

    for(int i = 0; i < num_params; i++){
        p[i] = 1;
    }
}