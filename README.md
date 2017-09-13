# Chronos
A fast Hamiltonian Monte Carlo sampler written in C.

## Building
Building the library is simply done by running cmake from the project root:
```
cmake .
make
```

## Testing
There are a couple of tests in the test directory. Parts of the test are written in C and must be compiled by running cmake in th test directory. After compiling the test library the python tests can be run.

### "normal" Test
Tests the code's ability to produce a standard normal distribution via the Box-Muller transformation. Should produce a plot of the generated standard normal distribution.

### "indep_normal" Test
Tests the sampler by sampling from an independent multivariate normal distribution. Should produce a plot of the log-likelihoods and a corner plot of the parameters with the true values indicated.
