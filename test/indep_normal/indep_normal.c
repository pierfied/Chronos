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
        inv_sigma_sqr = 1/(sigma[i] * sigma[i]);

        log_likelihood += diff * diff * inv_sigma_sqr;
        grad[i] = diff * inv_sigma_sqr;
    }
    log_likelihood *= -0.5;

    Hamiltonian H;
    H.log_likelihood = log_likelihood;
    H.grad = grad;

    return H;
}