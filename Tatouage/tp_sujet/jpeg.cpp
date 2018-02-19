#include "image.h"
#include "attacks.h"
#include "dct.h"
#include "mtrand.h"

int main(int argc, char** argv)
{
	if(argc!=3)
	{
		cout << argv[0] << " src.pgm QF\n"; 
		cout << "\t src.pgm  : image marquée\n"; 
		cout << "\t QF       : facteur de qualité entre 0 et 100\n\n"; 
		
		return -1; 
	}
	
	image<octet>	src, dst;
	image<double>	src_dct, dst_dct;
	
	long QF = strtol(argv[2], NULL, 10); 
	
	
	src.read(argv[1]); 

	dct::analyse(src, src_dct);

	attacks::jpeg(src_dct, dst_dct, QF);
	
	dct::synthese(dst_dct, dst); 
	
	dst.write(argv[1]); 
	
	return 1; 
}