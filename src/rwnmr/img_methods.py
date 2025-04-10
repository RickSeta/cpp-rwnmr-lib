
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
    print("Binarizing images....")
    img_array = np.array(img_array)
    for pixel in img_array:
        pixel[pixel != cor_poro] = 1
        pixel[pixel == cor_poro] = 0
    print("Images binarized")
    return img_array

