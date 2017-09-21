//
// Created by pierfied on 9/12/17.
//

#include "include/hmc.h"

typedef struct {
    int n_data;
    double *y;
    double *x;
    double *err;
} RPLikelihoodArgs;

typedef struct {
    double m_true;
    double b_true;
    double *y;
    double *x;
    double *err;
    SampleChain chain;
} RPSampleResults;

Hamiltonian RP_logp(double *, void *);

RPSampleResults RP_test(int n_data, int num_samples, int num_steps,
                   int num_burn, double epsilon);