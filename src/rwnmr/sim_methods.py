import rwnmr
from .config_classes import *
import numpy as np
def cpmg_simulation(cpmg_config:CpmgConfig, uct_config:UctConfig, rwnmr_config:RwnmrConfig, image:np.ndarray):
    if image.dtype != np.uint8:
        raise TypeError("Data must be an numpy array of dtype uint8")
    return rwnmr.wrapper.CPMG_EXECUTE(cpmg_config, rwnmr_config, uct_config, image,  image.shape[0], image.shape[1], image.shape[2] )