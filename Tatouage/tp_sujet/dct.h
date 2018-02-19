/*
	Block-DCT for the image template class - v. 0.4 - Mar. 09
 
	Copyright (c) 2009, Gaëtan Le Guelvouit <gaetan.leguelvouit@orange-ftgroup.com>
  
    Watson perceptual distance code by I. J. Cox, M. L. Miller and 
    J. A. Bloom, from "Digital watermarking" (Morgan Kaufmann) 
 
	This program is free software; you can redistribute it and/or
	modify it under the terms of the GNU General Public License as
	published by the Free Software Foundation; either version 2 of
	the License, or (at your option) any later version.
 
	This program is distributed in the hope that it will be
	useful, but WITHOUT ANY WARRANTY; without even the implied
	warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
	PURPOSE.  See the GNU General Public License for more details.
 
	You should have received a copy of the GNU General Public
	License along with this program; if not, write to the Free
	Software Foundation, Inc., 59 Temple Place, Suite 330, Boston,
	MA 02111-1307, USA.
*/        

#ifndef __DCT_H__
#define __DCT_H__

#include <math.h>

#include "common.h"
#include "image.h"

class dct
{
private:
	static void Data2DCT88(double* block);
	static void DCT882Data(double* block);
	
	static void Img2BlockDCT(const image<octet>& c, double* C, const int width, const int height);
	static void Img2BlockDCT(const image<double>& c, double* C, const int width, const int height);
	static void BlockDCT2Img(const image<double>& c, octet* C, const int width, const int height);
	static void BlockDCT2Img(const image<double>& c, double* C, const int width, const int height);
	
	static void GetWatsonSlks(const double* const C, double* s, const int width, const int height);
	static void GetWatsonSlksForOneBlock(const double* const C, const double C00, double* s);

public:
	// From an image to 8x8 DCT blocks
	static void analyse(const image<octet>& i, image<double>& d);
	
	// From an image to 8x8 DCT blocks
	static void analyse(const image<double>& i, image<double>& d);
	
	// From blocks to a image (IDCT)
	static void synthese(const image<double>& d, image<octet>& i);
	
	// From blocks to a image (IDCT)
	static void synthese(const image<double>& d, image<double>& i);
	
	// Compute Watson's perceptual weights
	static void watson(const image<double>& d, image<double>& s); 
};

#endif
