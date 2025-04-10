import glob
from pathlib import Path
import shlex
from setuptools import setup, Extension, find_packages
from setuptools.command.build_ext import build_ext
import os
import subprocess

source_files = glob.glob("src/**/*.cpp", recursive=True) + glob.glob("src/**/*.cu", recursive=True)

class CustomBuildExt(build_ext):
    def run(self):
        if not os.path.exists(self.build_temp):
            os.makedirs(self.build_temp)

        ext = self.extensions[0]
        for source in ext.sources:
            source = shlex.quote(source)
            build_temp = shlex.quote(os.path.abspath(self.build_temp))
            if source.endswith(".cu"):
                compile_command = f"nvcc -allow-unsupported-compiler -O3 -DTESTING_STENCIL -lcudart -lcuda -Xcompiler -fPIC,-fopenmp -c {source} -odir {build_temp}"
            elif source.endswith(".cpp"):
                if 'wrapper' not in source:
                    compile_command = f"nvcc -O3 -DTESTING_STENCIL -lcudart -lcuda  -Xcompiler -fPIC,-fopenmp -c \"{source}\" -odir {build_temp}"
                else:  # wrapper needs Python.h and numpy/core
                    
                    import numpy as np
                    include_paths = [
                        np.get_include(),
                        "/usr/include/python3.12",
                        "src/rwnmr/utils",
                        "src/rwnmr/math",
                        "src/rwnmr/framework",
                        "src/rwnmr/configFiles",
                    ]

                    self.include_dirs.extend(include_paths)
                    compile_command = f"nvcc -O3 -DTESTING_STENCIL -lcudart -lcuda -Xcompiler -fPIC,-fopenmp -c {source} -odir {build_temp}" + \
                        ' '.join([f" -I\"{include_dir}\"" for include_dir in self.include_dirs])
            else:  # skip other files
                continue
            print(compile_command)
            subprocess.check_call(compile_command, shell=True)
        ext.sources = []
        super().run()  

    def build_extensions(self):
        built_objects = glob.glob(os.path.join(self.build_temp, "*.o*"))
        lib_filename = os.path.join(self.build_lib, self.ext_map['rwnmr.wrapper']._file_name)
        Path(os.path.split(lib_filename)[0]).mkdir(parents=True, exist_ok=True)
        linker_command = rf"nvcc -Xcompiler -fopenmp -shared -o {lib_filename} {' '.join(built_objects)}" + \
            ' '.join([f" -L{library_dir}" for library_dir in self.library_dirs]) 
        print(linker_command)
        subprocess.check_call(linker_command, shell=True)
        super().build_extensions()

setup(
    name="rwnmr",
    version='2.0.1', 
    download_url="https://github.com/RickSeta/cpp-rwnmr-lib",
    author_email="rickseta@gmail.com",
    packages=find_packages(where="src"),
    package_dir={'': 'src/'},
    ext_modules=[
        Extension(
            "rwnmr.wrapper",  # Nome do módulo
            sources= source_files
            ,
            include_dirs=[
                'src/rwnmr/framework',
                'src/rwnmr/configFiles',
            ],
        )
    ],
    cmdclass={'build_ext': CustomBuildExt},
    install_requires=['numpy', 'pillow','wheel', 'setuptools'],
    setup_requires=['numpy', 'pillow','wheel', 'setuptools'],
)
#se não funcionar faça pip install --upgrade setuptools wheel 