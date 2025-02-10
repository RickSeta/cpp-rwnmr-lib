import rwnmr
import numpy as np

class UctConfig:
    def __init__(self, dir_path, filename, first_idx, digits, extension, slices, resolution, voxel_division, pore_color):
        self.dir_path = dir_path
        self.filename = filename
        self.first_idx = first_idx
        self.digits = digits
        self.extension = extension
        self.slices = slices
        self.resolution = resolution
        self.voxel_division = voxel_division
        self.pore_color = pore_color

class CpmgConfig:
    def __init__(self, apply_bulk, obs_time, method, time_verbose, residual_field, gradient_value, gradient_direction, path_to_field, interpolate_field, min_t2, max_t2, use_t2_logspace, num_t2_bins, min_lambda, max_lambda, num_lambdas, prune_num, noise_amp, save_mode, save_t2, save_walkers, save_decay, save_histogram, save_histogram_list):
        self.apply_bulk = apply_bulk
        self.obs_time = obs_time
        self.method = method
        self.time_verbose = time_verbose
        self.residual_field = residual_field
        self.gradient_value = gradient_value
        self.gradient_direction = gradient_direction
        self.path_to_field = path_to_field
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
    def __init__(self, map_time, map_steps, map_filter, map_tol, map_iterations, save_img_info, save_binimg, save_walkers, openmp_usage, openmp_threads, gpu_usage, reduce_in_gpu, walker_samples):
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

# Example usage
uct_config = UctConfig("path_to_folder_with_images", "imagefile_prefix", 0, 1, ".png", 1, 1.0, 0, 0)
cpmg_config = CpmgConfig("False", "3000.0", "image-based", "False", "uniform", "10.0", "2", "False", "0.1", "100000.0", "True", "256", "-4", "2", "512", "0", "0.00", "True", "False", "False", "False", "False", "False")
rwnmr_config = RwnmrConfig(0.0, 0, 0.0, 0.01, 1, False, False, False, True, 4, True, True, 1)

print(cpmg_config.path_to_field)
print(type(cpmg_config.path_to_field))
list = np.array([1.0,9.0],dtype= np.uint8)
print(rwnmr.CPMG(cpmg_config))
