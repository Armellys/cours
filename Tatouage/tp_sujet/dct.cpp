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

#include "dct.h"

#define max(a,b)   	((a)>(b)?(a):(b))
#define DESCALE(x) 	(((x)+4)>>3)


double alpha(unsigned int u, unsigned int N)
{
  if(u==0) return sqrt(1.0/N); else return sqrt(2.0/N);
}

void dct::Data2DCT88(double* block)
{
	static double divisors[8*8] = { 
		0.125, 0.09012, 0.0956709, 0.106304, 0.125, 0.159095, 0.23097, 0.453064, 
		0.09012, 0.0649729, 0.0689748, 0.0766407, 0.09012, 0.114701, 0.16652, 0.326641, 
		0.0956709, 0.0689748, 0.0732233, 0.0813614, 0.0956709, 0.121766, 0.176777, 0.34676, 
		0.106304, 0.0766407, 0.0813614, 0.0904039, 0.106304, 0.135299, 0.196424, 0.385299, 
		0.125, 0.09012, 0.0956709, 0.106304, 0.125, 0.159095, 0.23097, 0.453064, 
		0.159095, 0.114701, 0.121766, 0.135299, 0.159095, 0.202489, 0.293969, 0.576641, 
		0.23097, 0.16652, 0.176777, 0.196424, 0.23097, 0.293969, 0.426777, 0.837153, 
		0.453064, 0.326641, 0.34676, 0.385299, 0.453064, 0.576641, 0.837153, 1.64213
	};
 
	double  C0, C1, C2, C3, C4, C5, C6, C7, C10, C11, C12, C13;
	double  z1, z2, z3, z4, z5, z11, z13;
	double* dataptr;
	double  data[8*8];

	dataptr=data;
	for(int i=0; i<8; i++) 
	{
		*dataptr++=*block++;
		*dataptr++=*block++;
		*dataptr++=*block++;
		*dataptr++=*block++;
		*dataptr++=*block++;
		*dataptr++=*block++;
		*dataptr++=*block++;
		*dataptr++=*block++;
	}

	// Première passe : les lignes 
  dataptr=data;
  for(int ctr=7; ctr>=0; ctr--) 
  {
    C0=dataptr[0]+dataptr[7];
    C7=dataptr[0]-dataptr[7];
    C1=dataptr[1]+dataptr[6];
    C6=dataptr[1]-dataptr[6];
    C2=dataptr[2]+dataptr[5];
    C5=dataptr[2]-dataptr[5];
    C3=dataptr[3]+dataptr[4];
    C4=dataptr[3]-dataptr[4];
    
    // Partie paire
    C10=C0+C3; // Phase 2
    C13=C0-C3;
    C11=C1+C2;
    C12=C1-C2;
    
    dataptr[0]=C10+C11; // Phase 3	
    dataptr[4]=C10-C11;
    
    z1=(C12+C13)*((double)0.707106781);
    dataptr[2]=C13+z1; // Phase 5
    dataptr[6]=C13-z1;
    
 		// Parite impaire   
 		C10=C4+C5; // Phase 2
    C11=C5+C6;
    C12=C6+C7;

    z5=(C10-C12)*((double)0.382683433);
    z2=((double)0.541196100)*C10 + z5; 
    z4=((double)1.306562965)*C12 + z5;
    z3=C11*((double)0.707106781); 

    z11=C7+z3; // Phase 5
    z13=C7-z3;

    dataptr[5]=z13+z2; // Phase 6
    dataptr[3]=z13-z2;
    dataptr[1]=z11+z4;
    dataptr[7]=z11-z4;

    dataptr+=8;		
  }

	// Seconde passe : colonnes  
 	dataptr=data;
  for(int ctr=7; ctr>=0; ctr--) 
  {
    C0=dataptr[8*0]+dataptr[8*7];
    C7=dataptr[8*0]-dataptr[8*7];
    C1=dataptr[8*1]+dataptr[8*6];
    C6=dataptr[8*1]-dataptr[8*6];
    C2=dataptr[8*2]+dataptr[8*5];
    C5=dataptr[8*2]-dataptr[8*5];
    C3=dataptr[8*3]+dataptr[8*4];
    C4=dataptr[8*3]-dataptr[8*4];
    
  	// Partie impaire     
  	C10=C0+C3; // Phase 2
    C13=C0-C3;
    C11=C1+C2;
    C12=C1-C2;
    
    dataptr[8*0]=C10+C11; // Phase 3
    dataptr[8*4]=C10-C11;
    
    z1=(C12+C13)*((double)0.707106781);
    dataptr[8*2]=C13 + z1; // Phase 5
    dataptr[8*6]=C13 - z1;
    
   	// Partie impaire     
   	C10=C4+C5; // Phase 2
    C11=C5+C6;
    C12=C6+C7;

    z5=(C10-C12)*((double)0.382683433);
    z2=((double)0.541196100)*C10 + z5; 
    z4=((double)1.306562965)*C12 + z5;
    z3=C11*((double)0.707106781); 

    z11=C7+z3; // Phase 5
    z13=C7-z3;

    dataptr[8*5]=z13+z2; // Phase 6
    dataptr[8*3]=z13-z2;
    dataptr[8*1]=z11+z4;
    dataptr[8*7]=z11-z4;

    dataptr++;
  }

	for(int i=0; i<64; i++) block[i-64]=data[i]*divisors[i];
}


void dct::DCT882Data(double* block)
{
 	static double dct_table[8*8]= { 
 		1, 1.38704, 1.30656, 1.17588, 1, 0.785695, 0.541196, 0.275899, 
		1.38704, 1.92388, 1.81225, 1.63099, 1.38704, 1.08979, 0.750661, 0.382683, 
		1.30656, 1.81225, 1.70711, 1.53636, 1.30656, 1.02656, 0.707107, 0.36048, 
		1.17588, 1.63099, 1.53636, 1.38268, 1.17588, 0.92388, 0.636379, 0.324423, 
		1, 1.38704, 1.30656, 1.17588, 1, 0.785695, 0.541196, 0.275899, 
		0.785695, 1.08979, 1.02656, 0.92388, 0.785695, 0.617317, 0.425215, 0.216773, 
		0.541196, 0.750661, 0.707107, 0.636379, 0.541196, 0.425215, 0.292893, 0.149316, 
		0.275899, 0.382683, 0.36048, 0.324423, 0.275899, 0.216773, 0.149316, 0.0761205
	};

	double  C0, C1, C2, C3, C4, C5, C6, C7, C10, C11, C12, C13;
	double  z5, z10, z11, z12, z13;
	double* inptr;
	double* outptr;
	double* quantptr;
	double* wsptr;
	double  workspace[64];

	// Première passe : colonnes 
  quantptr=dct_table;
  inptr   =block;
  wsptr   =workspace;

  for(int ctr=8; ctr>0; ctr--) 
  {
    // Partie paire 
    C0=inptr[8*0]*(quantptr[8*0]);
    C1=inptr[8*2]*(quantptr[8*2]);
    C2=inptr[8*4]*(quantptr[8*4]);
    C3=inptr[8*6]*(quantptr[8*6]);

    C10=C0+C2; // Phase 3
    C11=C0-C2;

    C13=C1+C3; // Phases 5-3	
    C12=(C1-C3)*((double)1.414213562)-C13; 

    C0=C10+C13; // Phase 2
    C3=C10-C13;
    C1=C11+C12;
    C2=C11-C12;
    
		// Partie impaire 
    C4=inptr[8*1]*(quantptr[8*1]);
    C5=inptr[8*3]*(quantptr[8*3]);
    C6=inptr[8*5]*(quantptr[8*5]);
    C7=inptr[8*7]*(quantptr[8*7]);

    z13=C6+C5; // Phase 6
    z10=C6-C5;
    z11=C4+C7;
    z12=C4-C7;

    C7=z11+z13;	// Phase 5
    C11=(z11-z13)*((double)1.414213562); 

    z5=(z10+z12)*((double)1.847759065);
    C10=((double)1.082392200)*z12-z5; 
    C12=((double)-2.613125930)*z10+z5; 

    C6=C12-C7; // Phase 2
    C5=C11-C6;
    C4=C10+C5;

    wsptr[8*0]=C0+C7;
    wsptr[8*7]=C0-C7;
    wsptr[8*1]=C1+C6;
    wsptr[8*6]=C1-C6;
    wsptr[8*2]=C2+C5;
    wsptr[8*5]=C2-C5;
    wsptr[8*4]=C3+C4;
    wsptr[8*3]=C3-C4;

    inptr++;
    quantptr++;
    wsptr++;
  }
  
	// Seconde passe : colonnes 
	wsptr =workspace;
  outptr=block;
  for(int ctr=0; ctr<8; ctr++) 
  {
  	// Partie paire     
  	C10=wsptr[0]+wsptr[4];
    C11=wsptr[0]-wsptr[4];

    C13=wsptr[2]+wsptr[6];
    C12=(wsptr[2]-wsptr[6])*((double)1.414213562) - C13;

    C0=C10+C13;
    C3=C10-C13;
    C1=C11+C12;
    C2=C11-C12;

    // Partie impaire 
    z13=wsptr[5]+wsptr[3];
    z10=wsptr[5]-wsptr[3];
    z11=wsptr[1]+wsptr[7];
    z12=wsptr[1]-wsptr[7];

    C7=z11+z13;
    C11=(z11-z13)*((double)1.414213562);

    z5=(z10+z12)*((double)1.847759065);
    C10=((double)1.082392200)*z12-z5;
    C12=((double)-2.613125930)*z10+z5;

    C6=C12-C7;
    C5=C11-C6;
    C4=C10+C5;

    // Phase finale 
    outptr[0]=((int) DESCALE((int) (C0+C7)));
    outptr[7]=((int) DESCALE((int) (C0-C7)));
    outptr[1]=((int) DESCALE((int) (C1+C6)));
    outptr[6]=((int) DESCALE((int) (C1-C6)));
    outptr[2]=((int) DESCALE((int) (C2+C5)));
    outptr[5]=((int) DESCALE((int) (C2-C5)));
    outptr[4]=((int) DESCALE((int) (C3+C4)));
    outptr[3]=((int) DESCALE((int) (C3-C4)));
    
		outptr+=8;
    wsptr +=8;
	}
}
                              

void dct::Img2BlockDCT(const image<octet>& c, double* C, const int width, const int height)
{
	double block[8*8];

	for(int row=0; row<height/8; row++)
	{  
		for(int col=0; col<width/8; col++)
		{
			// Premier element de ce bloc
			const int i0=row*8*width+col*8;

			for(int y=0; y<8; y++)
			{
				for(int x=0; x<8; x++) block[8*y+x]=c[i0+y*width+x];
			}

			// Conversion du bloc 
			Data2DCT88(block);

			for(int y=0; y<8; y++)
			{
				for(int x=0; x<8; x++) C[i0+y*width+x]=block[8*y+x];
			}
		}
	}
}

void dct::Img2BlockDCT(const image<double>& c, double* C, const int width, const int height)
{
	double block[8*8];

	for(int row=0; row<height/8; row++)
	{  
		for(int col=0; col<width/8; col++)
		{
			// Premier element de ce bloc
			const int i0=row*8*width+col*8;

			for(int y=0; y<8; y++)
			{
				for(int x=0; x<8; x++) block[8*y+x]=c[i0+y*width+x];
			}

			// Conversion du bloc 
			Data2DCT88(block);

			for(int y=0; y<8; y++)
			{
				for(int x=0; x<8; x++) C[i0+y*width+x]=block[8*y+x];
			}
		}
	}
}

void dct::BlockDCT2Img(const image<double>& c, octet* C, const int width, const int height)
{
	double block[8*8];
	
	for(int row=0; row<height/8; row++)
	{
    for(int col=0; col<width/8; col++)
		{
			const int i0=row*8*width+col*8;

			for(int y=0; y<8; y++)
			{
				for(int x=0; x<8; x++) block[8*y+x]=c[i0+y*width+x];
			}

			DCT882Data(block);

			for(int y=0; y<8; y++)
			{
				for(int x=0; x<8; x++)
				{
				  if(block[8*y+x]>255) C[i0+y*width+x]=255; else 
				  {
				    if(block[8*y+x]<0) C[i0+y*width+x]=0; else C[i0+y*width+x]=int(0.5+block[8*y+x]);       
				  }
				}
			}
		}
	}
}

void dct::BlockDCT2Img(const image<double>& c, double* C, const int width, const int height)
{
	double block[8*8];
	
	for(int row=0; row<height/8; row++)
	{
    for(int col=0; col<width/8; col++)
		{
			const int i0=row*8*width+col*8;

			for(int y=0; y<8; y++)
			{
				for(int x=0; x<8; x++) block[8*y+x]=c[i0+y*width+x];
			}

			DCT882Data(block);

			for(int y=0; y<8; y++)
			{
				for(int x=0; x<8; x++)
				{
				  C[i0+y*width+x]=block[8*y+x];       
				}
			}
		}
	}
}

/*! Calcule les pondérations perceptuelles (modèle de Watson) pour un ensemble de blocs DCT.
  \param C Ensemble de blocs DCT 8x8.
  \param s Pondérations calculées.
  \param width Largeur en pixels de l'image.
  \param height Hauteur en pixels.*/
void dct::GetWatsonSlks(const double* const C, double* s, int width, int height)
{
  double CBlk[8*8];
  double sBlk[8*8];

  // Combien de blocs dans l'image ?
  int numBlkRows = height/8;
  int numBlkCols = width/8;

  // Moyenne des coef. DC
  double C00 = 0;
  
  for(int blkRow=0; blkRow<numBlkRows; blkRow++)
  {
    for(int blkCol=0; blkCol<numBlkCols; blkCol++) C00+=C[(blkRow*8 + 0)*width + (blkCol*8 + 0)];    
  }
  
  C00/=numBlkRows*numBlkCols; if(C00==0) C00=1; 
 
  // Initialisation des slacks 
  for(int i=0; i<width*height; i++) s[i]=1;

  // Boucle sur les blocs 
  for(int blkRow=0; blkRow<numBlkRows; blkRow++)
  {
    for(int blkCol=0; blkCol<numBlkCols; blkCol++)
    {
      /* Copy the block into a temporary, 8x8 array */
      for(int y=0; y<8; y++)
      {
        for(int x=0; x<8; x++) CBlk[y*8 + x]=C[(blkRow*8 + y)*width + (blkCol*8 + x)];           
      }
    
      // Calcul les pondérations pour ce block
      GetWatsonSlksForOneBlock(CBlk, C00, sBlk);

      // Recopie
      for(int y=0; y<8; y++)
      {
        for(int x=0; x<8; x++) s[(blkRow*8 + y)*width + (blkCol*8 + x)]=sBlk[y*8 + x];       
      }
    }
  }
}

/*! Compute Watson's weight for a given block.
  \param C 8x8 DCT block.
  \param C00 Mean value of DC samples.
  \param s Corresponding 8x8 block of weights.*/
void dct::GetWatsonSlksForOneBlock(const double* const C, const double C00, double* s)
{
  const static double t[8*8] =
  {
    1.404,  1.011,  1.169,  1.664,  2.408,  3.433,  4.796,  6.563,
    1.011,  1.452,  1.323,  1.529,  2.006,  2.716,  3.679,  4.939,
    1.169,  1.323,  2.241,  2.594,  2.988,  3.649,  4.604,  5.883,
    1.664,  1.529,  2.594,  3.773,  4.559,  5.305,  6.281,  7.600,
    2.408,  2.006,  2.988,  4.559,  6.152,  7.463,  8.713, 10.175,
    3.433,  2.716,  3.649,  5.305,  7.463,  9.625, 11.588, 13.519,
    4.796,  3.679,  4.604,  6.281,  8.713, 11.588, 14.500, 17.294,
    6.563,  4.939,  5.883,  7.600, 10.175, 13.519, 17.294, 21.156
  };

  double m; 

  for(int i=0; i<64; i++)
  {
    // Luminance masking 
    const double tL=t[i]*pow(C[ 0 ]/C00, 0.649);

    // Contrast masking 
    if(i==0) m=tL; else m=max(tL, pow(fabs(C[i]), 0.7)*pow(tL, 0.3 ));

    s[i]=m;
  }
}





/*! Compute DCT blocks.
  \param i Grayscale image.
  \param d DCT samples, organized in 8x8 blocks.
  \warning Only work for images with 8k dimensions. */
void dct::analyse(const image<octet>& i, image<double>& d)
{
	const unsigned int width =8*int(i.width()/8);
	const unsigned int height=8*int(i.height()/8);
	
	// Buffer temporaire pour les données DCT
	double* C=new double[width*height];
	
	Img2BlockDCT(i, C, width, height);

	// Recopie dans l'image finale	
	d.resize(width, height);
	d.init(C);
	
	delete [] C;
}

/*! Compute DCT blocks.
  \param i Grayscale image.
  \param d DCT samples, organized in 8x8 blocks.
  \warning Only work for images with 8k dimensions. */
void dct::analyse(const image<double>& i, image<double>& d)
{
	const unsigned int width =8*int(i.width()/8);
	const unsigned int height=8*int(i.height()/8);
	
	// Buffer temporaire pour les données DCT
	double* C=new double[width*height];
	
	Img2BlockDCT(i, C, width, height);

	// Recopie dans l'image finale	
	d.resize(width, height);
	d.init(C);
	
	delete [] C;
}

/*! Inverse block-DCT.
  \param d Set of 8x8 blocks.
  \param i Resulting image. */
void dct::synthese(const image<double>& d, image<octet>& i)
{
	const unsigned int width =8*int(d.width()/8);
	const unsigned int height=8*int(d.height()/8);
	
	// Temp. buffer 
	octet* C=new octet[width*height];
	
	BlockDCT2Img(d, C, width, height);

	// Copy in the final image 	
	i.resize(width, height);
	i.init(C);
	
	delete [] C;
}

/*! Inverse block-DCT.
  \param d Set of 8x8 blocks.
  \param i Resulting image. */
void dct::synthese(const image<double>& d, image<double>& i)
{
	const unsigned int width =8*int(d.width()/8);
	const unsigned int height=8*int(d.height()/8);
	
	// Temp. buffer 
	double* C=new double[width*height];
	
	BlockDCT2Img(d, C, width, height);

	// Copy in the final image 	
	i.resize(width, height);
	i.init(C);
	
	delete [] C;
}


/*! Compute Watson's perceptual weigths from a set of DCT blocks.
  \param d DCT blocks.
  \param s Resulting weigths. */
void dct::watson(const image<double>& d, image<double>& s)
{
	double* S = new double[d.size()];
	double* C = new double[d.size()];
	
	for(unsigned int j=0; j<d.size(); j++) C[j]=d[j];
	
	GetWatsonSlks(C, S, d.width(), d.height());
	
	s.resize(d.width(), d.height());
	s.init(S);
	
	delete [] S;
	delete [] C;
} 

