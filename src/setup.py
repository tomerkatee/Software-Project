from setuptools import Extension, setup

module = Extension("mykmeanssp", sources=['spkmeansmodule.c', 'spkmeans.c', 'clustering.c', 'jacobi.c', 'matrix.c'])
setup(name='mykmeanssp',
     version='1.0',
     description='Python wrapper for custom C extension',
     ext_modules=[module])