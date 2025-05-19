
import rwnmr
import numpy as np
from PIL import Image
from os import listdir
import matplotlib.pyplot as plt
from rwnmr.config_classes import *

import pickle


uct_config = UctConfig(
    first_idx="0",
    digits="1",
    extension="0",
    slices="700",
    resolution="1.0",
    voxel_division="0",
    pore_color="0"
)
cpmg_config = CpmgConfig(
    apply_bulk="false",
    obs_time="1000.0",
    method="image-based",
    time_verbose="false",
    residual_field="uniform",
    gradient_value="10.0",
    gradient_direction="2",
    interpolate_field="false",
    min_t2="0.1",
    max_t2="100000.0",
    use_t2_logspace="true",
    num_t2_bins="256",
    min_lambda="-4",
    max_lambda="2",
    num_lambdas="512",
    prune_num="0",
    noise_amp="0.00",
    save_mode="true",
    save_t2="true",
    save_walkers="true",
    save_decay="true",
    save_histogram="true",
    save_histogram_list="true"
)

rwnmr_config = rwnmr.RwnmrConfig(
    name="acbez",
    map_time="0.0",
    map_steps="0",
    map_filter="0.0",
    map_tol="0.01",
    map_iterations="1",
    save_img_info="false",
    save_binimg="false",
    save_walkers="false",
    openmp_usage="true",
    openmp_threads="4",
    gpu_usage="true",
    reduce_in_gpu="true",
    walker_samples="1",
    walkers="655360",
    walkers_placement="random",
    placement_deviation="0.0",
    rho_type="uniform",
    rho="{23.3}",
    giromagnetic_ratio="42.576",
    giromagnetic_unit="mhertz",
    d0="2.5",
    bulk_time="2600.0",
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
print(type(mat))
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
    print(img_array)
    img_array[img_array != cor_poro] = 1
    img_array[img_array == cor_poro] = 0
    print("Images binarized")
    self.img_array = img_array
    return img_array, rows, cols

# img = binarize(uct_config, "./imgs/AC_bez", 1, 0, 0)
# print(img)
# exit(0)
# loadcsv = np.loadtxt("FEM_T2_AC.csv", delimiter=",")
x,y =(rwnmr.sim_methods.cpmg_simulation(cpmg_config, uct_config, rwnmr_config, "./imgs/AC_bez"))
# x,y =(rwnmr.sim_methods.cpmg_simulation(cpmg_config, uct_config, rwnmr_config, "./imgs/sphere100"))

pickle.dump(x, open("resultX", "wb"))
pickle.dump(y, open("resultY", "wb"))

# x = pickle.load(open("resultX", "rb"))
# y = pickle.load(open("resultY", "rb"))


print("\n\n\n\nimg:" + rwnmr_config.name)
# print("img:" + uct_config.img_array.shape)
plt.plot(x, y)
import matplotlib.pyplot as plt


# plt.plot(y,(-y/0.267))
# plt.plot(y,np.log(x))

plt.title("CPMG Simulation")
plt.xlabel("Time")
plt.ylabel("Amplitude")
plt.grid(True)
plt.show()
# plt.plot(y, (x))
# x_csv = loadcsv[:,0]
# y_csv = loadcsv[:,1]
# plt.plot(x_csv*1000, y_csv)


from nmrinv_main import simple_nmr_t2_inversion

t2 ,ft2, c = simple_nmr_t2_inversion(x, y, 0.1)
plt.semilogx(t2, ft2)

plt.xlabel("Time")
plt.ylabel("Amplitude")
plt.show()


