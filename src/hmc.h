//
// Created by pierfied on 9/10/17.
//

#ifndef CHRONOS_HMC_H
#define CHRONOS_HMC_H

/*
 * Struct containing the arguments for the HMC sampler.
 *
 * log_likelihood: Function pointer to the log-likelihood calculation.
 * likelihood_args: Arguments to be passed to the likelihood function.
 * num_samples: The number of samples to generate.
 * num_params: The number of parameters being sampled over.
 * num_steps: The number of leapfrog steps between samples.
 * epsilon: Step-size
 * x0: The initial position of the sampler.
 */
typedef struct {
    double *(*log_likelihood)(void *);
    void *likelihood_args;
    int num_samples;
    int num_params;
    int num_steps;
    double epsilon;
    double *x0;
} HMCArgs;

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
