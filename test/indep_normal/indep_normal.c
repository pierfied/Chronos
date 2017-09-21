//
// Created by pierfied on 9/12/17.
//

#include <stdlib.h>
#include "indep_normal.h"
#include "include/hmc.h"
#include "include/normal.h"

Hamiltonian logp(double *x, void *arg_ptr){
    INLikelihoodArgs *args = (INLikelihoodArgs *)arg_ptr;

    int num_params = args->num_params;
    double *mu = args->mu;
    double *sigma = args->sigma;

    double log_likelihood = 0;
    double diff,inv_sigma_sqr;
    double *grad = malloc(sizeof(double) * num_params);
    for(int i = 0; i < num_params; i++){
        diff = (x[i] - mu[i]);
        inv_sigma_sqr = 1.0/(sigma[i] * sigma[i]);

        log_likelihood += diff * diff * inv_sigma_sqr;
        grad[i] = -diff * inv_sigma_sqr;
    }
    log_likelihood *= -0.5;

    Hamiltonian H;
    H.log_likelihood = log_likelihood;
    H.grad = grad;

    return H;
}

SampleResults test(int num_params, int num_samples, int num_steps,
                   int num_burn, double epsilon){
    INLikelihoodArgs args;
    args.num_params = num_params;

    double *mu = malloc(sizeof(double) * num_params);
    double *sigma = malloc(sizeof(double) * num_params);
    double x0[num_params];
    for(int i = 0; i < num_params; i++){
        mu[i] = normal();
        sigma[i] = 1 + 0.25 * normal();
        x0[i] = normal();
    }
    args.mu = mu;
    args.sigma = sigma;

    HMCArgs hmc_args;
    hmc_args.log_likelihood = logp;
    hmc_args.likelihood_args = (void *)&args;
    hmc_args.num_samples = num_samples;
    hmc_args.num_params = num_params;
    hmc_args.num_steps = num_steps;
    hmc_args.num_burn = num_burn;
    hmc_args.epsilon = epsilon;
    hmc_args.x0 = x0;

    SampleChain chain = hmc(hmc_args);
    SampleResults results;
    results.chain = chain;
    results.x_true = mu;

    return results;
}