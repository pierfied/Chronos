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
class RPSampleResults(ctypes.Structure):
    _fields_ = [('m_true', ctypes.c_double),
                ('b_true', ctypes.c_double),
                ('y', ctypes.POINTER(ctypes.c_double)),
                ('x', ctypes.POINTER(ctypes.c_double)),
                ('err', ctypes.POINTER(ctypes.c_double)),
                ('chain', SampleChain)]


# Load the test library.
tests = ctypes.cdll.LoadLibrary('../libtests.so')

# Define the test function call.
test = tests.RP_test
test.argtypes = [ctypes.c_int, ctypes.c_int, ctypes.c_int, ctypes.c_int, ctypes.c_double]
test.restype = RPSampleResults

# Run the sampler on the test.
num_data = 10
num_samps = int(1e5)
num_burn = int(1e4)
results = test(num_data, num_samps, 1, num_burn, 0.05)
print('Acceptance Rate: ', str(results.chain.accept_rate))

# Extract the chain data from the returned results.
x_true = np.array([results.m_true, results.b_true])
chain = np.array([[results.chain.samples[i][j] for j in range(2)] for i in range(num_samps)])
likelihoods = np.array([results.chain.log_likelihoods[i] for i in range(num_samps)])

# Extract the test data from the returned results.
x = np.array([results.x[i] for i in range(num_data)])
y = np.array([results.y[i] for i in range(num_data)])
err = np.array([results.err[i] for i in range(num_data)])

# Plot the test data.
plt.errorbar(x, y, yerr=err, fmt='o')
plt.show()

# Should see a nice converged chain.
plt.plot(range(len(likelihoods)), likelihoods)
plt.show()

# Should see independent random normal variables.
corner(chain, truths=x_true, labels=('m', 'b'), levels=(0.68, 0.95,))
plt.show()
