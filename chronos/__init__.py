import os, glob, cffi

#Figure out the directories to everything.
chronos_dir = os.path.dirname(__file__)
include_dir = os.path.join(chronos_dir,'include')
#This will be the name of the library file that gets compiled.
lib_file = os.path.join(chronos_dir,'_chronos.so')

#Use cffi.FFI().cdef() to be able to call the functions
#specified in the header files. This is necessary.
_ffi = cffi.FFI()
for file_name in glob.glob(os.path.join(include_dir,'*.h')):
    _ffi.cdef(open(file_name).read())
_lib = _ffi.dlopen(lib_file)

#Import everything from test. This isn't necessary,
#since we could call chronos._lib.myfunc() directly,
#but I wrote nice wrappers in the .test module.
#This is also how you would get documentation to appear,
#since it won't be generated directly from the headers.
from .test import *
