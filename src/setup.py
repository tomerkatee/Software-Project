from setuptools import Extension, setup

module = Extension("spkmeans_module", sources=['spkmeansmodule.c', 'spkmeans.c', 'clustering.c', 'jacobi.c', 'matrix.c'])
setup(name='spkmeans_module',
     version='1.0',
     description='Python wrapper for custom C extension',
     ext_modules=[module])