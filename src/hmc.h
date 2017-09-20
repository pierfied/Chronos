//
// Created by pierfied on 9/10/17.
//

#ifndef CHRONOS_HMC_H
#define CHRONOS_HMC_H

/*
 * Struct containing information about the Hamiltonian.
 *
 * H: Hamiltonian
 * K: Kinetic Term
 * log_likelihood: Full log_likelihood.
 * grad: Gradient of the likelihood for each parameter.
 */
typedef struct {
    double H;
    double K;
    double log_likelihood;
    double *grad;
} Hamiltonian;

/*
 * Struct containing the arguments for the HMC sampler.
 *
 * log_likelihood: Function pointer to the log-likelihood calculation.
 *     This function must return a double array of size 1+num_params where
 *     the first element is the log-likelihood followed by the gradients.
 * likelihood_args: Arguments to be passed to the likelihood function.
 * num_samples: The number of samples to generate.
 * num_params: The number of parameters being sampled over.
 * num_steps: The number of leapfrog steps between samples.
 * epsilon: Step-size
 * x0: The initial position of the sampler.
 */
typedef struct {
    Hamiltonian (*log_likelihood)(double *, void *);

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
 * log_likelihoods: Log-likelihood value for each sample.
 */
typedef struct {
    int num_samples;
    int num_params;
    double accept_rate;
    double **samples;
    double *log_likelihoods;
} SampleChain;

SampleChain hmc(HMCArgs hmc_args);

double *init_p(int num_params);

Hamiltonian hamiltonian(double *x, double *p, HMCArgs hmc_args);

Hamiltonian log_likelihood(double *x, HMCArgs hmc_args);

void update_hamiltonian_momenta(double *p, Hamiltonian *H,
                                HMCArgs hmc_args);

void leapfrog_update(double *x, double *p, double *grad, int num_params,
                     double epsilon);

#endif //CHRONOS_HMC_H
