import ctypes
import numpy as np
import matplotlib.pyplot as plt
from chainconsumer import ChainConsumer
import emcee

class SampleChain(ctypes.Structure):
    _fields_ = [('num_samples', ctypes.c_int),
                ('num_params', ctypes.c_int),
                ('accept_rate', ctypes.c_double),
                ('samples', ctypes.POINTER(ctypes.POINTER(ctypes.c_double))),
                ('log_likelihoods', ctypes.POINTER(ctypes.c_double))]

class ETSampleResults(ctypes.Structure):
    _fields_ = [('chain', SampleChain)]

tests = ctypes.cdll.LoadLibrary('../libtests.so')

test = tests.ET_test
test.argtypes = [ctypes.c_int,ctypes.POINTER(ctypes.c_double),
                 ctypes.POINTER(ctypes.c_double),
                 ctypes.POINTER(ctypes.c_double),
                 ctypes.c_int,ctypes.c_int,ctypes.c_int,
                 ctypes.c_double]
test.restype = ETSampleResults

# General params
num_data = 10
num_samps = int(1e5)
num_burn = int(1e4)
num_steps = 1
epsilon = 0.05

# Line params
m_true = 2
b_true = -1
err_mean = 0.75
err_sigma = 0.25
x_true = np.array([m_true,b_true])

# Generate a data sample.
x = np.arange(num_data,dtype=np.float64)
err = err_mean + err_sigma * np.random.randn(num_data)
y = m_true*x + b_true + err * np.random.randn(num_data)

plt.errorbar(x,y,yerr=err,fmt='o')
plt.title('Data Sample')
plt.show()

# Likelihood for emcee.
def lnprob(params,x,y,err):
    m = params[0]
    b = params[1]

    y_model = m*x + b

    return -0.5 * np.sum(((y - y_model)/err)**2)

# Emcee params
num_dim = 2
num_walkers = 10
p0 = [np.random.randn(num_dim) for i in range(num_walkers)]

# Perform sampling with emcee.
sampler = emcee.EnsembleSampler(num_walkers,num_dim,lnprob,args=[x,y,err])
sampler.run_mcmc(p0,num_samps/num_walkers)

# Burn in
samples = sampler.chain[:,100:,:].reshape((-1,num_dim))

# Get the pointers to the data.
x_dat = x.ctypes.data_as(ctypes.POINTER(ctypes.c_double))
y_dat = y.ctypes.data_as(ctypes.POINTER(ctypes.c_double))
err_dat = err.ctypes.data_as(ctypes.POINTER(ctypes.c_double))

# Run the HMC
results = test(num_data,x_dat,y_dat,err_dat,
               num_samps,num_steps,num_burn,epsilon)
print('Acceptance Rate: ',str(results.chain.accept_rate))

# Get the results.
chain = np.array([[results.chain.samples[i][j] for j in range(2)]
         for i in range(num_samps)])
likelihoods = np.array([results.chain.log_likelihoods[i] for i
                        in range(num_samps)])

# Should see a nice converged chain.
plt.plot(range(len(likelihoods)),likelihoods)
plt.title('Log-Likelihood')
plt.xlabel('Step \#')
plt.ylabel('$\ln(P)$')
plt.show()

# Should match the distribution from emcee.
c = ChainConsumer()
c.add_chain(samples,['m','b'],name='emcee')
c.add_chain(chain,['m','b'],name='HMC')
c.configure(sigma2d=False)
fig = c.plotter.plot(figsize='column',truth=x_true)
plt.tight_layout()
plt.savefig('emcee_compare.png')
plt.show()