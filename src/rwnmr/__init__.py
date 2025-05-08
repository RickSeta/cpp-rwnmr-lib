"""
Random Walk NMR Simulator
=========================

A Python package for NMR simulations using random walk methods.
"""

# Import configuration classes
from .config_classes import UctConfig, CpmgConfig, RwnmrConfig

# Import simulation methods
from .sim_methods import cpmg_simulation

# Import image processing methods
from .img_methods import ler_imagem, binarize

# Import C++ wrapper functionality
try:
    from . import wrapper
    # Expose wrapper functions directly in the rwnmr namespace
    CPMG_EXECUTE = wrapper.CPMG_EXECUTE
except ImportError as e:
    raise ImportError(
        "Failed to import rwnmr C++ extension. Make sure the package is properly installed "
        "and all dependencies are satisfied. Original error: " + str(e)
    )

# Define what gets imported with "from rwnmr import *"
__all__ = [
    'UctConfig',
    'CpmgConfig',
    'RwnmrConfig',
    'cpmg_simulation',
    'ler_imagem',
    'binarize',
    'CPMG_EXECUTE'
]