import ctypes

class SampleChain(ctypes.Structure):
    _fields_ = [('num_samples', ctypes.c_int),
                ('num_params', ctypes.c_int),
                ('accept_rate', ctypes.c_double),
                ('samples', ctypes.POINTER(ctypes.POINTER(ctypes.c_double))),
                ('log_likelihoods', ctypes.POINTER(ctypes.c_double))]

class SampleResults(ctypes.Structure):
    _fields_ = [('x_true', ctypes.POINTER(ctypes.c_double)),
                ('chain', SampleChain)]

tests = ctypes.cdll.LoadLibrary('../libtests.so')

test = tests.test
test.argtypes = [ctypes.c_int,ctypes.c_int,ctypes.c_int,ctypes.c_double]
test.restype = SampleResults

print(test(10,10,1,0.1))