from pybm3d_gpu import bm3d
import numpy as np
import skimage.data
from skimage.measure import compare_psnr as psnr

img = skimage.data.imread("lena.png")[..., None]
img_noisy = skimage.data.imread("lena_20.png")[..., None]

img_denoised = np.asarray(bm3d.bm3d(img_noisy, 20))
#z = pybm3d.bm3d.bm3d(img_noisy.astype(np.float32), 20.0)
print(psnr(img, img_noisy))
print(psnr(img, img_denoised))
