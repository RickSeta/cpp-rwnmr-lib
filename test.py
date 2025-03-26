import rwnmr
import numpy as np
from PIL import Image
from os import listdir

def ler_imagem(img_dir,num_slices, slice_inicial=0):
    lista_slices = listdir(img_dir)
    img_array =[]
    for slice in range(slice_inicial, num_slices):
        img= np.array(Image.open(img_dir +"/" + lista_slices[slice]).convert("L"),dtype=np.uint8)

        img_array.append(img)
    
    return img_array

def binarize(img_array, cor_poro):
    img_array = np.array(img_array)
    print(cor_poro)
    # print("==============")
    # print(img_array)
    # print("==============")
    for pixel in img_array:
        pixel[pixel != cor_poro] = 0
        pixel[pixel == cor_poro] = 1

    # print("==============")
    # print(img_array)
    # print("==============")
    return img_array

class UctConfig:
    def __init__(self, first_idx, digits, extension, slices, resolution, voxel_division, pore_color):

        self.first_idx = first_idx
        self.digits = digits
        self.extension = extension
        self.slices = slices
        self.resolution = resolution
        self.voxel_division = voxel_division
        self.pore_color = pore_color 

class CpmgConfig:
    def __init__(self, apply_bulk, obs_time, method, time_verbose, residual_field, gradient_value, gradient_direction, interpolate_field, min_t2, max_t2, use_t2_logspace, num_t2_bins, min_lambda, max_lambda, num_lambdas, prune_num, noise_amp, save_mode, save_t2, save_walkers, save_decay, save_histogram, save_histogram_list):
        self.apply_bulk = apply_bulk
        self.obs_time = obs_time
        self.method = method
        self.time_verbose = time_verbose
        self.residual_field = residual_field
        self.gradient_value = gradient_value
        self.gradient_direction = gradient_direction
        self.interpolate_field = interpolate_field
        self.min_t2 = min_t2
        self.max_t2 = max_t2
        self.use_t2_logspace = use_t2_logspace
        self.num_t2_bins = num_t2_bins
        self.min_lambda = min_lambda
        self.max_lambda = max_lambda
        self.num_lambdas = num_lambdas
        self.prune_num = prune_num
        self.noise_amp = noise_amp
        self.save_mode = save_mode
        self.save_t2 = save_t2
        self.save_walkers = save_walkers
        self.save_decay = save_decay
        self.save_histogram = save_histogram
        self.save_histogram_list = save_histogram_list

class RwnmrConfig:
    def __init__(self, name, map_time, map_steps, map_filter, map_tol, map_iterations, save_img_info, save_binimg, save_walkers, openmp_usage, openmp_threads, gpu_usage, reduce_in_gpu, walker_samples, walkers, walkers_placement, placement_deviation, rho_type, rho, giromagnetic_ratio, giromagnetic_unit, d0, bulk_time, steps_per_echo, bc, histograms, histogram_size, histogram_scale, blocks, threads_per_block, echoes_per_kernel, max_rwsteps ,seed):
        self.name = name
        self.map_time = map_time
        self.map_steps = map_steps
        self.map_filter = map_filter
        self.map_tol = map_tol
        self.map_iterations = map_iterations
        self.save_img_info = save_img_info
        self.save_binimg = save_binimg
        self.save_walkers = save_walkers
        self.openmp_usage = openmp_usage
        self.openmp_threads = openmp_threads
        self.gpu_usage = gpu_usage
        self.reduce_in_gpu = reduce_in_gpu
        self.walker_samples = walker_samples
        self.walkers = walkers
        self.walkers_placement = walkers_placement
        self.placement_deviation = placement_deviation
        self.rho_type = rho_type
        self.rho = rho
        self.giromagnetic_ratio = giromagnetic_ratio
        self.giromagnetic_unit = giromagnetic_unit
        self.d0 = d0
        self.bulk_time = bulk_time
        self.steps_per_echo = steps_per_echo
        self.bc = bc
        self.histograms = histograms
        self.histogram_size = histogram_size
        self.histogram_scale = histogram_scale
        self.blocks = blocks
        self.threads_per_block = threads_per_block
        self.echoes_per_kernel = echoes_per_kernel
        self.max_rwsteps = max_rwsteps
        self.seed = seed
# Example usage
uct_config = UctConfig("10", "1", ".png", "1", "1.0", "0", "0")
cpmg_config = CpmgConfig("False", "3000.0", "image-based", "False", "uniform", "10.0", "2", "False", "0.1", "100000.0", "True", "256", "-4", "2", "512", "0", "0.00", "True", "False", "False", "False", "False","False")
rwnmr_config = RwnmrConfig(
    name="examplename",
    map_time="0.0",
    map_steps="0",
    map_filter="0.0",
    map_tol="0.01",
    map_iterations="1",
    save_img_info="False",
    save_binimg="False",
    save_walkers="False",
    openmp_usage="True",
    openmp_threads="4",
    gpu_usage="True",
    reduce_in_gpu="True",
    walker_samples="1",
    walkers="1000",
    walkers_placement="random",
    placement_deviation="0.0",
    rho_type="uniform",
    rho="{0.0}",
    giromagnetic_ratio="42.576",
    giromagnetic_unit="mhertz",
    d0="2.5",
    bulk_time="2800.0",
    steps_per_echo="4",
    bc="mirror",
    histograms="1",
    histogram_size="256",
    histogram_scale="linear",
    blocks="4096",
    threads_per_block="1024",
    echoes_per_kernel="16",
    max_rwsteps="65536",
    seed="0"
    
)

# print(cpmg_config.path_to_field)
# print(type(cpmg_config.path_to_field))
mat = np.array([[
                    [0, 0, 0, 0,   0, 1, 0, 0],
                    [0, 0, 0, 0,   0, 0, 0, 0],
                    [0, 0, 0, 0,   0, 0, 0, 0],
                    [0, 0, 0, 0,   0, 0, 0, 0],
                    
                    [0, 0, 0, 0,   0, 0, 0, 0],
                    [0, 0, 0, 0,   0, 0, 0, 0],
                    [0, 0, 0, 0,   0, 0, 0, 0],
                    [0, 0, 0, 0,   0, 0, 0, 0],
                ],[
                    [1, 0, 0, 0,   1, 0, 0, 0],
                    [0, 0, 0, 0,   0, 0, 0, 0],
                    [0, 0, 0, 0,   0, 0, 0, 0],
                    [0, 0, 0, 0,   0, 0, 0, 0],
                    
                    [0, 0, 0, 0,   0, 0, 0, 0],
                    [0, 0, 0, 0,   0, 0, 0, 0],
                    [0, 0, 0, 0,   0, 0, 0, 0],
                    [0, 0, 0, 0,   0, 0, 0, 0],
                ],[
                    [0, 0, 0, 0,   0, 0, 0, 0],
                    [0, 0, 0, 0,   0, 0, 0, 0],
                    [0, 0, 0, 0,   0, 0, 0, 0],
                    [0, 0, 0, 0,   0, 0, 0, 0],
                    
                    [0, 0, 0, 0,   0, 0, 0, 0],
                    [0, 0, 0, 0,   0, 0, 0, 0],
                    [0, 0, 0, 0,   0, 0, 0, 0],
                    [0, 0, 0, 0,   0, 0, 0, 0],
                ],[
                    [0, 0, 0, 0,   0, 0, 0, 0],
                    [0, 0, 0, 0,   0, 0, 0, 0],
                    [0, 0, 0, 0,   0, 0, 0, 0],
                    [0, 0, 0, 0,   0, 0, 0, 0],
                    
                    [0, 0, 0, 0,   0, 0, 0, 0],
                    [0, 0, 0, 0,   0, 0, 0, 0],
                    [0, 0, 0, 0,   0, 0, 0, 0],
                    [0, 0, 0, 0,   0, 0, 0, 0],
                ],], dtype=np.uint8) 

# rwnmr.BitBlockMethod(mat, mat.shape[0], mat.shape[1], mat.shape[2],)
# print(rwnmr.CPMG(cpmg_config))
# print(rwnmr.RWNMR(rwnmr_config))
# print(rwnmr.UCT(uct_config))
# binarized = binarize(ler_imagem("./img_reader/testsimgs", 2, 0), 255)
# print(binarized)
# rwnmr.BitBlockMethod(binarized, binarized.shape[0], binarized.shape[1], binarized.shape[2],)
rwnmr.CPMG_EXECUTE(cpmg_config, rwnmr_config, uct_config, mat,  mat.shape[0], mat.shape[1], mat.shape[2] )