# Chronos
A fast Hamiltonian Monte Carlo sampler written in C.

## Building
Building the library is simply done by running cmake from the project root:
```
cmake .
make
```

## Testing
There are a couple of tests in the test directory. Parts of the test are written in C and must be compiled by running cmake in the test directory first. After compiling the test library the python tests can be run.

### "normal" Test
Tests the code's ability to produce a standard normal distribution via the Box-Muller transformation. Should produce a plot of the generated standard normal distribution.

Run from the ```test/normal``` directory:
```
python normal.py
```

### "indep_normal" Test
Tests the sampler by sampling from an independent multivariate normal distribution. Should produce a plot of the log-likelihoods and a corner plot of the parameters with the true values indicated.

Run from the ```test/indep_normal``` directory:
```
python indep_normal.py
```

### "recover_params" Test
Tests the sampler by recovering the slope and intercept parameters for a simple linear fit to observations with error bars. Should produce a plot of the log-likelihoods and a corner plot of the parameters with the true values indicated.

Run from the ```test/recover_params``` directory:
```
python recover_params.py
```

### "emcee_test" Test
Similar to the "recover_params" except compares output chain from HMC to that of emcee. Should produce a plot of the log-likelihoods and a corner plot of the parameters with the true values indicated.

Run from the ```test/recover_params``` directory:
```
python recover_params.py
```
