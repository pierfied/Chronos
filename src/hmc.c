//
// Created by pierfied on 9/10/17.
//

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
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
    int num_params = hmc_args.num_params;
    int num_samps = hmc_args.num_samples;
    int num_steps = hmc_args.num_steps;
    double epsilon = hmc_args.epsilon;

    int num_accept = 0;
    double **samples = malloc(sizeof(double *) * num_samps);
    double *log_likelihoods = malloc(sizeof(double *) * num_samps);

    // Initialize the positions and momenta.
    double *x = malloc(sizeof(double) * num_params);
    double *x_prime = malloc(sizeof(double) * num_params);
    for (int i = 0; i < num_params; i++) {
        x[i] = hmc_args.x0[i];
    }

    // Generate the samples.
    for (int i = 0; i < num_samps; i++) {
        // Copy initial positions and generate new momenta.
        for (int j = 0; j < num_params; j++) {
            x_prime[j] = x[j];
        }
        double *p = init_p(num_params);

        // Compute the initial Hamiltonian and gradients.
        Hamiltonian H = hamiltonian(x, p, hmc_args);
        Hamiltonian H_prime;
        double *grad = H.grad;

        // Perform the leapfrog propagation.
        for (int j = 0; j < num_steps; j++) {
            leapfrog_update(x_prime, p, grad, num_params, epsilon);

            free(grad);

            H_prime = hamiltonian(x_prime, p, hmc_args);
            grad = H_prime.grad;
        }

        free(grad);

        // Perform Metropolis-Hastings update.
        double accept_prob = fmin(1, exp(H.H - H_prime.H));
        if (drand48() <= accept_prob) {
            double *tmp = x;
            x = x_prime;
            x_prime = tmp;
            H.log_likelihood = H_prime.log_likelihood;
            num_accept++;
        }
        samples[i] = malloc(sizeof(double) * num_params);
        for (int j = 0; j < num_params; j++) {
            samples[i][j] = x[j];
        }
        log_likelihoods[i] = H.log_likelihood;

        free(p);
    }

    free(x);
    free(x_prime);

    // Create the chain return struct.
    SampleChain chain;
    chain.num_samples = num_samps;
    chain.num_params = num_params;
    chain.accept_rate = ((double) num_accept) / num_samps;
    chain.samples = samples;
    chain.log_likelihoods = log_likelihoods;

    return chain;
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
    for (int i = 0; i < num_params; i++) {
        p[i] = normal();
    }

    return p;
}

/*
 * hamiltonian
 * Computes the Hamiltonian of the current state.
 *
 * Params:
 * x: Current positions.
 * p: Current momenta.
 * hmc_args: HMCArgs struct with appropriate likelihood function.
 *
 * Returns:
 * Hamiltonian of the current state.
 */
Hamiltonian hamiltonian(double *x, double *p, HMCArgs hmc_args) {
    Hamiltonian log_p = hmc_args.log_likelihood(x, hmc_args.likelihood_args);

    // Compute the kinetic contribution the Hamiltonian.
    double K = 0;
    for (int i = 0; i < hmc_args.num_params; i++) {
        K += p[i] * p[i];
    }
    log_p.K = K;
    log_p.H = K - log_p.log_likelihood;

    return log_p;
}

/*
 * leapfrog_update
 * Perform a leapfrog step updating both position and momenta.
 *
 * Params:
 * x: The current positions.
 * p: The current momenta.
 * grad: The potential gradients at the current position.
 * num_params: Number of parameters (size of x).
 * epsilon: Step-size
 */
void leapfrog_update(double *x, double *p, double *grad, int num_params,
                     double epsilon) {
    double half_epsilon = 0.5 * epsilon;

    for (int i = 0; i < num_params; i++) {
        // Perform a half step in momentum.
        p[i] += half_epsilon * grad[i];

        // Perform a full step in position.
        x[i] += epsilon * p[i];

        // Perform another half step in momentum.
        p[i] += half_epsilon * grad[i];
    }
}