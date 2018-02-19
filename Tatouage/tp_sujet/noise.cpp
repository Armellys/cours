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
		cout << "\t n        : niveau de bruit\n\n"; 
		
		return -1; 
	}
	
	image<octet>	src, dst;
	
	long n = strtol(argv[2], NULL, 10); 
	
	
	src.read(argv[1]); 


	attacks::noise(src, dst, n);
	
	
	dst.write(argv[1]); 
	
	return 1; 
}