//
// Created by pierfied on 9/12/17.
//

typedef struct {
    int num_params;
    double *mu;
    double *sigma;
} INLikelihoodArgs;

Hamiltonian logp(double *, void *);