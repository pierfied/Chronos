import ctypes
import numpy as np
import matplotlib.pyplot as plt
from corner import corner


# Struct returned by HMC sampler.
class SampleChain(ctypes.Structure):
    _fields_ = [('num_samples', ctypes.c_int),
                ('num_params', ctypes.c_int),
                ('accept_rate', ctypes.c_double),
                ('samples', ctypes.POINTER(ctypes.POINTER(ctypes.c_double))),
                ('log_likelihoods', ctypes.POINTER(ctypes.c_double))]


# Struct returned by test code.
class SampleResults(ctypes.Structure):
    _fields_ = [('x_true', ctypes.POINTER(ctypes.c_double)),
                ('chain', SampleChain)]


# Load the test library.
tests = ctypes.cdll.LoadLibrary('../libtests.so')

# Define the test function.
test = tests.test
test.argtypes = [ctypes.c_int, ctypes.c_int, ctypes.c_int, ctypes.c_int,
                 ctypes.c_double]
test.restype = SampleResults

# Run the test.
num_params = 10
num_samps = int(1e5)
num_burn = int(1e4)
results = test(num_params, num_samps, 10, num_burn, 1e-2)
print('Acceptance Rate: ', str(results.chain.accept_rate))

# Extract the chain data from the returned results.
x_true = np.array([results.x_true[i] for i in range(num_params)])
chain = np.array([[results.chain.samples[i][j] for j in range(num_params)] for i in range(num_samps)])
likelihoods = np.array([results.chain.log_likelihoods[i] for i in range(num_samps)])

# Should see a nice converged chain.
plt.plot(range(len(likelihoods)), likelihoods)
plt.show()

# Should see independent random normal variables.
corner(chain, truths=x_true)
plt.show()
