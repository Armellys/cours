/*
	Some attacks for watermark benchmarking - v. 0.2 - Sep. 04
 
	Copyright (c) 2004, Gaëtan Le Guelvouit <gaetan.leguelvouit@orange-ftgroup.com>
 
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

#ifndef __ATTACKS_H__
#define __ATTACKS_H__

#include <math.h>
#include <stdlib.h>

#include "common.h"
#include "image.h"

class attacks
{
public:
	// Add Gaussian noise (spatial domain)
	static void noise(const image<octet>& a, image<octet>& b, const double sd);

	// A JPEG quantization (DCT domain)
	static void jpeg(const image<double>& a, image<double>& b, const unsigned int QF);
	
	// Gaussian filtering (spatial domain)
	static void filtering(const image<octet>& a, image<octet>& b, const double sg);

	// Valumetric scaling (spatial domain)
	static void scale(const image<octet>& a, image<octet>& b, const double s);
};



#endif
