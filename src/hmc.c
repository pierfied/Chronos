//
// Created by pierfied on 9/10/17.
//

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

/*
 * init_p
 * Initializes the momenta from a standard normal distribution.
 *
 * Params:
 * num_params: The number of parameters to generate momenta for.
 *
 * Returns:
 * An array of momenta values with length num_params.
 */
double *init_p(int num_params) {
    double *p = malloc(sizeof(double) * num_params);
    for(int i = 0; i < num_params; i++){
        p[i] = normal();
    }

    return p;
}