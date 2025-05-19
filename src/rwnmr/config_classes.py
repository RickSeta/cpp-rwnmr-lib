import numpy as np
from os import listdir
from PIL import Image
class UctConfig:
    def __init__(self, first_idx, digits, extension, slices, resolution, voxel_division, pore_color):

        self.first_idx = first_idx
        self.digits = digits
        self.extension = extension
        self.slices = slices
        self.resolution = resolution
        self.voxel_division = voxel_division
        self.pore_color = pore_color
        self.img_array = np.array([])
    def binarize(self, path, num_slices,slice_inicial, cor_poro):
        lista_slices = listdir(path)
        img_array = np.array([], dtype=np.uint8)
        print("Binarizing images....")

        img= Image.open(path +"/" + lista_slices[0]).convert("L")
        cols = img.size[0]
        rows = img.size[1]
        for slice in range(int(slice_inicial), int(num_slices)):
            img= Image.open(path +"/" + lista_slices[slice]).convert("L")
            img_array = np.append(img_array, np.array(img, dtype=np.uint8))
        print("Images loaded")
        img_array[img_array != cor_poro] = 1
        img_array[img_array == cor_poro] = 0
        print("Images binarized")
        self.img_array = img_array
        return img_array, rows, cols


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
