# distutils: language = c++

from libcpp.vector cimport vector
from libcpp cimport bool
cimport numpy as np
import numpy as np

cdef extern from "../src/main.cpp":
    int start_bm3d(vector[unsigned char] & img_in,
                   vector[unsigned char] & img_out,
                   const unsigned int height,
                   const unsigned int width,
                   const unsigned int channels,
                   const float sigma,
                   const bool verbose)

cpdef unsigned char[:, :, :] bm3d(unsigned char[:, :, :] img_in,
                                  float sigma,
                                  bool verbose=False):
    cdef vector[unsigned char] c_img_in
    cdef vector[unsigned char] c_img_out

    height = img_in.shape[0]
    width = img_in.shape[1]
    channels = img_in.shape[2]

    c_img_in.resize(img_in.size)
    pos = 0
    for h in range(height):
        for w in range(width):
            for c in range(channels):
                c_img_in[pos] = img_in[h, w, c]
                pos +=1

    status = start_bm3d(c_img_in,
                        c_img_out,
                        height,
                        width,
                        channels,
                        sigma,
                        verbose)

    cdef np.ndarray img_out = np.zeros([height, width, channels], dtype=np.uint8)

    pos = 0
    for h in range(height):
        for w in range(width):
            for c in range(channels):
                img_out[h, w, c] = c_img_out[pos]
                pos +=1

    return img_out

