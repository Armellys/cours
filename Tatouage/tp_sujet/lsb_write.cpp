#include "image.h"
#include "mtrand.h"

int main(int argc, char** argv)
{
	if(argc!=4)
	{
		cout << argv[0] << " src1.pnm src2.pnm dst.pnm\n"; 
		cout << "\t src1.pnm  : image hôte\n"; 
		cout << "\t src2.pgm  : image secrête\n"; 
		cout << "\t dst.pnm   : image marquée\n\n"; 
		
		return -1; 
	}
	
	mtsrand(78425UL);
	
	image<rgb> 		src1; 
	image<octet>	src2;
	
	src1.read(argv[1]); 
	src2.read(argv[2]);
	
	for(int y=0; y<src1.height(); y++)
	{
		for(int x=0; x<src1.width(); x++)
		{
			src1(x, y).g = (src1(x, y).g >> 1) << 1; //met le bit de poid faible à 0
			
			const bool b = src2(x%src2.width(), y%src2.height()) > 128; //on va cacher une image en noir et blanc, si c'est au dessus de 128 ON MET UN 1 ET UN 0 SINON 

			if(mtrand()%2)
			{
				if(b) src1(x, y).g++; 
			}
			else 
			{
				if(!b) src1(x, y).g++; 
			}
		}
	}
	
	src1.write(argv[3]); 
	
	return 1; 
}
