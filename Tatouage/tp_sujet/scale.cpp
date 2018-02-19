#include "image.h"
#include "attacks.h"
#include "dct.h"
#include "mtrand.h"

int main(int argc, char** argv)
{
	if(argc!=3)
	{
		cout << argv[0] << " src.pnm QF\n"; 
		cout << "\t src.pnm  : image marquée\n"; 
		cout << "\t scale    : facteur d'échelle x100\n\n"; 
		
		return -1; 
	}
	
	image<octet>	src, dst;
	image<double>	src_dct, dst_dct;
	
	double scale = double(strtol(argv[2], NULL, 10))/100.0;  
	
	
	src.read(argv[1]); 


	attacks::scale(src, dst, scale);
	
	dst.write(argv[1]); 
	
	return 1; 
}