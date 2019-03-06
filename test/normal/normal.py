import numpy as np
import matplotlib.pyplot as plt
from ctypes import cdll, c_double

# Load the chronos library.
chronos = cdll.LoadLibrary('../../libchronos.so')
normal = chronos.normal
normal.restype = c_double

# Generate random numbers from c code.
num_samps = 100000
samps = np.zeros(num_samps)
for i in range(num_samps):
    samps[i] = normal()

# Should look like a standard normal distribution.
plt.hist(samps, 50)
plt.show()
