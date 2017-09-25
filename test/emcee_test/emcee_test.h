//
// Created by pierfied on 9/12/17.
//

#include "include/hmc.h"

typedef struct {
    int n_data;
    double *y;
    double *x;
    double *err;
} ETLikelihoodArgs;

typedef struct {
    SampleChain chain;
} ETSampleResults;

Hamiltonian ET_logp(double *, void *);

ETSampleResults ET_test(int n_data, double *x, double *y, double *err,
                        int num_samples, int num_steps,
                        int num_burn, double epsilon);