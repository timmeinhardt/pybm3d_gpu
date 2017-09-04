#include <iostream>
#include <string>

#include "bm3d.hpp"
#define cimg_display 0
#include "CImg.h"

using namespace cimg_library;

int start_bm3d(std::vector<unsigned char> & img_in,
               std::vector<unsigned char> & img_out,
               const unsigned int height,
               const unsigned int width,
               const unsigned int channels,
               const float sigma,
               const bool verbose)
{
	bool twostep = true;

    if (verbose)
	{
		std::cout << "Sigma = " << sigma << std::endl;
		if (twostep)
			std::cout << "#Steps: 2" << std::endl;
		else
			std::cout << "#Steps: 1" << std::endl;

		if (channels > 1)
			std::cout << "Color denoising: yes" << std::endl;
		else
			std::cout << "Color denoising: no" << std::endl;
	}

	//Allocate images
	CImg<unsigned char> c_img_in(img_in.data(), width, height, 1, channels, false);
	CImg<unsigned char> c_img_out(width, height, 1, channels, false);

	//Convert color image to YCbCr color space
	if (channels == 3)
		c_img_in = c_img_in.get_channels(0, 2).RGBtoYCbCr();

	if(verbose)
		std::cout << "width: " << width << " height: " << height << std::endl;

	//Launch BM3D
	try {
		BM3D bm3d;
		//		    (n, k,N, T,   p,sigma, L3D)
		bm3d.set_hard_params(19, 8, 16, 2500, 3, sigma, 2.7f);
		bm3d.set_wien_params(19, 8, 32, 400, 3, sigma);
		bm3d.set_verbose(verbose);
		bm3d.denoise_host_image(c_img_in.data(),
				                c_img_out.data(),
				                width,
				                height,
				                channels,
				                twostep);
	}
	catch(std::exception & e)  {
		std::cerr << "There was an error while processing image: "
                  << std::endl << e.what() << std::endl;
		return 1;
	}

	if (channels == 3) //color
		//Convert back to RGB color space
		c_img_out = c_img_out.get_channels(0,2).YCbCrtoRGB();
	else
		c_img_out = c_img_out.get_channel(0);

    img_out.assign(c_img_out.data(),
                   c_img_out.data() + (height * width * channels));

    return 0;
}
