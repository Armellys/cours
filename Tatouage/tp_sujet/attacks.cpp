/*
	Some attacks for watermark benchmarking - v. 0.4 - Mar. 10
 
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

#include "attacks.h"

/*! Simulate a JPEG compression.
  \param a Image of block-DCT samples.
  \param b Quantized DCT samples.
  \param QF JPEG quality factor (0-100). */
void attacks::jpeg(const image<double>& a, image<double>& b, const unsigned int QF)
{
	// The famous JPEG quantization table 
	static int t[8*8] =
  {
    16, 11, 10, 16, 24, 40, 51, 61,
    12, 12, 14, 19, 26, 58, 60, 55,
    14, 13, 16, 24, 40, 57, 69, 56,
    14, 17, 22, 29, 51, 87, 80, 62, 
    18, 22, 37, 56, 98, 109, 103, 77,
    24, 35, 55, 64, 81, 104, 113, 92, 
    48, 64, 78, 87, 103, 121, 120, 101, 
    72, 92, 95, 98, 112, 100, 103, 99
  };

	if(QF>=100) b=a;
	else
	{
		double Q = 0;
	
		// Quantization level from quality factor 
		if(QF<50) Q=50.0/QF; else Q=2.0-0.02*QF;
	
		b.resize(a.width(), a.height()); 
	
		for(unsigned int y=0; y<a.height(); y++)
		{
			for(unsigned int x=0; x<a.width(); x++)
			{
				unsigned int   bx  = x%8;
				unsigned int   by  = y%8;  
				double q   = Q*t[8*by + bx];
		
				if(a(x, y)>0) b(x, y)=q*int(a(x, y)/q + 0.5); else b(x, y)=q*int(a(x, y)/q - 0.5); 
			}
		}
	}
}

/*! Add Gaussian noise.
  \param a Input image (spatial domain).
  \param b Noisy image.
  \param sd Standard deviation of the desired attack noise. */
void attacks::noise(const image<octet>& a, image<octet>& b, const double sd)
{
	b.resize(a.width(), a.height());

	for(unsigned int y=0; y<a.height(); y++)
	{
		for(unsigned int x=0; x<a.width(); x++)
		{
			const double x1 = ((double)rand()+1.)/((double)RAND_MAX+1.);
  			const double x2 = (double)rand()/(double)RAND_MAX;

  			const double g  = sd*sqrt(-2.0*log(x1))*cos(2.0*M_PI*x2);
		
			const double n =  g + a(x, y);
			
			if(n>255) b(x, y); else
			{
				if(n<0) b(x, y)=0; else b(x, y)=int(0.5 + n);
			}
		}
	}
}

/*! Valuemetric scaling. 
  \param a Input image.
  \param b Scaled image. 
  \param s Scale factor (e.g. 0 leads to a black image). */
void attacks::scale(const image<octet>& a, image<octet>& b, const double s)
{
	b.resize(a.width(), a.height());

	for(unsigned int y=0; y<a.height(); y++)
	{
		for(unsigned int x=0; x<a.width(); x++)
		{
			const double n =  s*a(x, y);
			
			if(n>255) b(x, y); else
			{
				if(n<0) b(x, y)=0; else b(x, y)=int(0.5 + n);
			}
		}
	}
}

/*! Gaussian blurring filter.
  \param a Input image.
  \param b Filtered image. 
  \param sg Standard deviation of the Gaussian filter. */
void attacks::filtering(const image<octet>& a, image<octet>& b, const double sg)
{
	if(sg<=0) b=a;
	else
	{
		b.resize(a.width(), a.height());

		for(int y=0; y<a.height(); y++)
		{
			for(int x=0; x<a.width(); x++)
			{
				double s = 0;
				double t = 0;
		
				for(int dy=-4; dy<5; dy++)
				{
					for(int dx=-4; dx<5; dx++)
					{
						double d2 = dx*dx + dy*dy;
						double c  = 1.0/(sg*sg*6.2831853) * exp(-d2/(2.0*sg*sg));
					
						if((x+dx>=0) && (y+dy>=0) && (x+dx<a.width()) && (y+dy<a.height()))
						{ 
							t+=c*a(x+dx, y+dy);
							s+=c;
						}
					}
				}
			
				b(x, y)=int(0.5 + t/s);
			}
		}
	}
}
