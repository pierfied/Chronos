//
// Created by pierfied on 9/12/17.
//

#include <stdlib.h>
#include <time.h>
#include "emcee_test.h"
#include "include/hmc.h"
#include "include/normal.h"
#include <stdio.h>

Hamiltonian ET_logp(double *params, void *arg_ptr){
    ETLikelihoodArgs *args = (ETLikelihoodArgs *)arg_ptr;

    int n_data = args->n_data;
    double *y = args->y;
    double *x = args->x;
    double *err = args->err;

    double m = params[0];
    double b = params[1];
    double y_model;
    double log_likelihood = 0;
    double *grad = malloc(sizeof(double) * 2);
    grad[0] = 0;
    grad[1] = 0;
    for(int i = 0; i < n_data; i++){
        y_model = m * x[i] + b;

        log_likelihood += (y[i] - y_model)*(y[i] - y_model)/(err[i] * err[i]);

        grad[0] += (y[i] - y_model)*x[i]/(err[i] * err[i]);
        grad[1] += (y[i] - y_model)/(err[i] * err[i]);
    }
    log_likelihood *= -0.5;

    Hamiltonian H;
    H.log_likelihood = log_likelihood;
    H.grad = grad;

    return H;
}

ETSampleResults ET_test(int n_data, double *x, double *y, double *err,
                        int num_samples, int num_steps,
                        int num_burn, double epsilon){
    srandom(time(NULL));

    ETLikelihoodArgs args;
    args.n_data = n_data;
    args.x = x;
    args.y = y;
    args.err = err;

    int num_params = 2;
    double params0[2];
    params0[0] = normal();
    params0[1] = normal();

    HMCArgs hmc_args;
    hmc_args.log_likelihood = ET_logp;
    hmc_args.likelihood_args = (void *)&args;
    hmc_args.num_samples = num_samples;
    hmc_args.num_params = num_params;
    hmc_args.num_steps = num_steps;
    hmc_args.num_burn = num_burn;
    hmc_args.epsilon = epsilon;
    hmc_args.x0 = params0;

    SampleChain chain = hmc(hmc_args);
    ETSampleResults results;
    results.chain = chain;

    return results;
}