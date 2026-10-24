from setuptools import Extension, setup

'''Define the C extension module'''
module = Extension(
    "symnmfmodule", 
    sources=['symnmfmodule.c', 'symnmf.c']
)

setup(
    name='symnmfmodule',
    version='1.0',
    description='SymNMF C extension for Python',
    ext_modules=[module]
)