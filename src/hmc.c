//
// Created by pierfied on 9/10/17.
//

#include <stdlib.h>
#include <math.h>
#include <time.h>
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
    // Seed the random number generator.
    srandom(time(NULL));

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

        // Perform the first step in leapfrog.
        double half_epsilon = 0.5*epsilon;
        for(int j = 0; j < num_params; j++){
            // Half-step in momenta.
            p[j] += half_epsilon * grad[j];

            // Full-step in position.
            x_prime[j] += epsilon * p[j];
        }
        free(grad);

        // Update the gradients.
        H_prime = log_likelihood(x_prime, hmc_args);
        grad = H_prime.grad;

        // Perform the rest of the leapfrog propagation.
        for (int j = 1; j < num_steps; j++) {
            for(int k = 0; k < num_params; k++){
                // Full-step in momenta.
                p[k] += epsilon * grad[k];

                // Full-step in position.
                x_prime[k] += epsilon * p[k];
            }
            free(grad);

            // Update the gradients.
            H_prime = log_likelihood(x_prime, hmc_args);
            grad = H_prime.grad;
        }

        // Perform the last half-step in momenta.
        for(int j = 0; j < num_params; j++){
            p[j] += half_epsilon * grad[j];
        }
        free(grad);

        // Update the proposed Hamiltonian with the current momenta.
        update_hamiltonian_momenta(p, &H_prime, hmc_args);

        // Perform Metropolis-Hastings update.
        double accept_prob = fmin(1, exp(H.H - H_prime.H));
        if (random()/(double) RAND_MAX <= accept_prob) {
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
    Hamiltonian log_p = log_likelihood(x, hmc_args);
    update_hamiltonian_momenta(p, &log_p, hmc_args);

    return log_p;
}

/*
 * log_likelihood
 * Computes the log-likelihood of the current state.
 *
 * Params:
 * x: Current positions.
 * hmc_args: HMCArgs struct with appropriate likelihood function.
 *
 * Returns:
 * Hamiltonian struct with only likelihood and gradient values.
 */
Hamiltonian log_likelihood(double *x, HMCArgs hmc_args){
    Hamiltonian log_p = hmc_args.log_likelihood(x, hmc_args.likelihood_args);
    return log_p;
}

/*
 * update_hamiltonian_momenta
 * Updates the hamiltonian passed in to include the kinetic
 * contribution from the provided momenta.
 *
 * Params:
 * p: Pointer to momenta.
 * H: Pointer to the Hamiltonian struct to be updated.
 * hmc_args: HMCArgs struct with appropriate likelihood function.
 */
void update_hamiltonian_momenta(double *p, Hamiltonian *H,
                                       HMCArgs hmc_args){
    // Compute the kinetic contribution to the Hamiltonian.
    double K = 0;
    for (int i = 0; i < hmc_args.num_params; i++) {
        K += p[i] * p[i];
    }
    K *= 0.5;
    H->K = K;
    H->H = K - H->log_likelihood;
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