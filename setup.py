from setuptools import setup
from setuptools import Extension
import numpy


setup(
    name="rwnmr",
    version=1,
    ext_modules=[
        Extension(
            "rwnmr",  # Nome do módulo
            ['src/rwnmr/framework/rwnmr_wrapper.cpp','src/rwnmr/configFiles/CpmgConfig.cpp','src/rwnmr/configFiles/RwnmrConfig.cpp'], # Arquivos C
            include_dirs=[numpy.get_include(), 'src/rwnmr/framework','/usr/include/eigen3'],  # Inclua o diretório do seu cabeçalho Defs.h
            extra_compile_args=['-std=c99'], # Adicione esta linha para usar o padrão C99
            language="c++", # Adicione esta linha para usar o compilador C++
        )
    ],
    install_requires=['numpy'],
    setup_requires=['numpy'],
    # Adicione as linhas abaixo para garantir que o setuptools encontre o compilador C++
    build_requires=['setuptools>=61.0'], # Ou a versão mais recente
)
#se não funcionar faça pip install --upgrade setuptools wheel 