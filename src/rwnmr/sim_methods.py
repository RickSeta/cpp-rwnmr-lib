import rwnmr
from .config_classes import *
import numpy as np
def cpmg_simulation(cpmg_config:CpmgConfig, uct_config:UctConfig, rwnmr_config:RwnmrConfig, path:str):
    image, rows, cols = uct_config.binarize(path, uct_config.slices, uct_config.first_idx, int(uct_config.pore_color))
    if not isinstance(image, np.ndarray):
        raise TypeError("Data must be an numpy array")
    if image.dtype != np.uint8:
        raise TypeError("Data must be an numpy array of dtype uint8")
    x,y = rwnmr.CPMG_EXECUTE(cpmg_config, rwnmr_config, uct_config, image, int(uct_config.slices),  rows, cols )
    return np.array(x), np.array(y)