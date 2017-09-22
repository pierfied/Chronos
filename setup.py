"""
The setup file is where the c code will get compiled.

All of this can be copied, you just need an identical
directory structure for your project, or you can mess with
that if you want.

Many, many thanks to Mike Jarvis, who implemented this
technique for Treecorr and Coord.
https://github.com/rmjarvis/TreeCorr
https://github.com/LSSTDESC/Coord/tree/master/coord

Check out those projects to see how you can
facilitate requirements and more dependencies.
"""
from __future__ import print_function
import sys, os, glob
import setuptools
from setuptools import setup, Extension

#Create the symlink.
try:
    os.symlink('../include/', 'chronos/include')
except:
    OSError
    
#Specify the sources
sources = glob.glob(os.path.join('src','*.c'))
print('sources = ',sources)
#and the header files.
headers = glob.glob(os.path.join('include','*.h'))
print('headers = ',headers)
#Create the extension.
ext=Extension("chronos._chronos", sources, depends=headers, include_dirs=['include'])

#Create the distribution.
dist = setup(name="chronos",
             author="Pier Fiedorosomething",
             author_email="peirs.email@gmail.com",
             description="A Python wrapped fast HMC code implemented in C.",
             license="MIT License",
             url="https://github.com/tmcclintock/Chronos",
             packages=['chronos'],
             package_data={'chronos' : headers },
             ext_modules=[ext])

#setup.py doesn't put the .so file in the chronos directory, 
#so this bit makes it possible to
#import chronos from the root directory.  
#Not really advisable, but everyone does it at some
#point, so might as well facilitate it.
build_lib = glob.glob(os.path.join('build','*','chronos','_chronos*.so'))
if len(build_lib) >= 1:
    lib = os.path.join('chronos','_chronos.so')
    if os.path.lexists(lib): os.unlink(lib)
    print(build_lib)
    os.link(build_lib[0], lib)
