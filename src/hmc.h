//
// Created by pierfied on 9/10/17.
//

#ifndef CHRONOS_HMC_H
#define CHRONOS_HMC_H

/*
 * Struct containing information about the sample chain
 * as well as the full Markov chain.
 *
 * accept_rate: The fraction of proposal steps accepted.
 * samples: Markov chain of shape num_samples x num_params.
 */
typedef struct {
    int num_samples;
    int num_params;
    double accept_rate;
    double **samples;
} SampleChain;

#endif //CHRONOS_HMC_H
