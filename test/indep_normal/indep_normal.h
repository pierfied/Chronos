//
// Created by pierfied on 9/12/17.
//

#include "include/hmc.h"

typedef struct {
    int num_params;
    double *mu;
    double *sigma;
} INLikelihoodArgs;

typedef struct {
    double *x_true;
    SampleChain chain;
} SampleResults;

Hamiltonian logp(double *, void *);

SampleResults test(int num_params, int num_samples, int num_steps,
                   int num_burn, double epsilon);