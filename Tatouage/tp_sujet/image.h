/*
	A simple image template class - v. 0.3 - Mar. 09
 
	Copyright (c) 2009, Gaëtan Le Guelvouit <gaetan.leguelvouit@orange-ftgroup.com>
 
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


#ifndef __IMAGE_TC_H__
#define __IMAGE_TC_H__

#include <fstream>
#include <iostream>

#include "common.h"

using namespace std; 

// A simple type for grayscale images 
#define octet unsigned char 

// A color pixel. 
class rgb 
{
public:
	octet r, g, b;  

  	rgb() : r(0), g(0), b(0) {};

  	rgb(const octet R, const octet G, const octet B) : r(R), g(G), b(B) {};

  	rgb(const octet val) : r(val), g(val), b(val) {};

	rgb(const double Y, const double U, const double V) : r(), g(), b() {}; 
 
  	rgb& operator=(const rgb &a) {r=a.r; g=a.g; b=a.b; return *this;};

	double Y() {return 0.299*r + 0.587*g + 0.114*b; };
	
	double U() {return -0.147*r - 0.289*g + 0.436*b; };
	
	double V() {return 0.615*r - 0.515*g - 0.100*b; };
	
	void setYUV(const double Y, const double U, const double V)
	{
		int ir = int(0.5 + Y + 1.140*V); 
		int ig = int(0.5 + Y - 0.395*U - 0.581*V);
		int ib = int(0.5 + Y + 2.032*U);
		
		if(ir<0) ir=0; else if(ir>255) ir=255; 
		if(ig<0) ig=0; else if(ig>255) ig=255;
		if(ib<0) ib=0; else if(ib>255) ib=255;
		
		r=ir; g=ig; b=ib;
	};
};


template <class T> 
class image
{
protected:
	int w, h;   // Height and width  
	T*  buffer; // Data 

	static void skip_space(istream &is); 

public:
/* Constructors */
	// Default 
  image() : w(0), h(0), buffer(NULL) {};

	// With the size of the image 
  image(const int n_width, const int n_height);
	image(const int n_width, const int n_height, const T &val);

	// Copy constructor 
  image(const image &img);

	// Copy from a data buffer  
  image(const int n_width, const int n_height, const T* const buf);

   // Destructor
  ~image();


/* Accessors */
  // Return width 
  int width() const {return w;};

  // Height
  int height() const {return h;};

  // Size of the whole image (i.e. number of pixels)
  int size() const {return w*h;};

/* Reading data access */
	// From X and Y 
  const T& operator()(const int x, const int y) const {return buffer[x + w*y];};

	// From the pixel number  
  const T& operator[](const int i) const {return buffer[i];};

/* Writing data access */
  T& operator()(const int x, const int y) {return buffer[x + w*y];};

  T& operator[](const int i) {return buffer[i];};


/* Misc. */
	// Affectation
  image& operator=(const image &img);

  // Init the whole image with a common value 
  void init(const T &val);

  // Init with a data buffer  
  void init(const T* const buf) {for(int i=0; i<w*h; i++) buffer[i]=buf[i];};

  // Set a new size  
  void resize(const int n_width, const int n_height);
  
  
/* I/O */
  // Read from a file (PPM or PGM format)
  int read(const char* const file);
  
  // Write to a file  
  int write(const char* const file) const;
};

#include "image.cpp"

#endif 


