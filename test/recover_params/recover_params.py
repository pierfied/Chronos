import ctypes
import numpy as np
import matplotlib.pyplot as plt
from corner import corner

class SampleChain(ctypes.Structure):
    _fields_ = [('num_samples', ctypes.c_int),
                ('num_params', ctypes.c_int),
                ('accept_rate', ctypes.c_double),
                ('samples', ctypes.POINTER(ctypes.POINTER(ctypes.c_double))),
                ('log_likelihoods', ctypes.POINTER(ctypes.c_double))]

class RPSampleResults(ctypes.Structure):
    _fields_ = [('m_true', ctypes.c_double),
                ('b_true', ctypes.c_double),
                ('y', ctypes.POINTER(ctypes.c_double)),
                ('x', ctypes.POINTER(ctypes.c_double)),
                ('err', ctypes.POINTER(ctypes.c_double)),
                ('chain', SampleChain)]

tests = ctypes.cdll.LoadLibrary('../libtests.so')

test = tests.RP_test
test.argtypes = [ctypes.c_int,ctypes.c_int,ctypes.c_int,ctypes.c_double]
test.restype = RPSampleResults

num_data = 10
num_samps = int(1e5)
results = test(num_data,num_samps,1,1e-2)
print('Acceptance Rate: ',str(results.chain.accept_rate))

# x_true = np.array([results.x_true[i] for i in range(num_data)])
x_true = np.array([results.m_true,results.b_true])
chain = np.array([[results.chain.samples[i][j] for j in range(2)]
         for i in range(num_samps)])
chain = chain[int(1e4):,:]
likelihoods = np.array([results.chain.log_likelihoods[i] for i
                        in range(num_samps)])

x = np.array([results.x[i] for i in range(num_data)])
y = np.array([results.y[i] for i in range(num_data)])
err = np.array([results.err[i] for i in range(num_data)])

plt.errorbar(x,y,yerr=err,fmt='o')
plt.show()

# Should see a nice converged chain.
plt.plot(range(len(likelihoods)),likelihoods)
plt.show()

# Should see independent random normal variables.
corner(chain,truths=x_true,labels=('m','b'),levels=(0.68,0.95,))
plt.show()