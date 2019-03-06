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
 *     This function must take a double pointer containing the current parameter values and a void pointer containing
 *     the arguments for the likelihood function.
 * likelihood_args: Arguments to be passed to the likelihood function.
 * num_params: The number of parameters being sampled over.
 * num_burn: The number of samples to be generated for burn-in. Burn-in samples will not be returned.
 * num_burn_steps: The number of leapfrog steps to be taken in a burn-in sample.
 * burn_epsilon: Step-size to be used during burn-in.
 * num_samples: The number of samples to generate.
 * num_steps: The number of leapfrog steps to be taken in a sample of the chain.
 * epsilon: Step-size to be used during sampling of the chain.
 * x0: The initial parameter values for the sampler.
 * m: Masses for each parameter.
 * sigma_p: Standard deviations from which to draw momenta for each parameter.
 * verbose: If set to 1 will print out the step number and acceptance probability for each step.
 */
typedef struct {
    Hamiltonian (*log_likelihood)(double *, void *);

    void *likelihood_args;
    int num_params;
    int num_burn;
    int num_burn_steps;
    double burn_epsilon;
    int num_samples;
    int num_samp_steps;
    double samp_epsilon;
    double *x0;
    double *m;
    double *sigma_p;
    int verbose;
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

double *init_p(int num_params, double *sigma_p);

Hamiltonian hamiltonian(double *x, double *p, double *inv_m, HMCArgs hmc_args);

Hamiltonian log_likelihood(double *x, HMCArgs hmc_args);

void update_hamiltonian_momenta(double *p, Hamiltonian *H, double *inv_m, HMCArgs hmc_args);

#endif //CHRONOS_HMC_H
