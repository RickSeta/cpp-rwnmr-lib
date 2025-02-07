from setuptools import setup
from setuptools import Extension
import numpy 

setup(
    name="pcteste",
    version=1,
    ext_modules=[Extension("modulo1", ['imgC.c'],include_dirs=[numpy.get_include()])],
    install_requires=['numpy'],
    setup_requires=['numpy'],

)
#pip install --upgrade setuptools wheel