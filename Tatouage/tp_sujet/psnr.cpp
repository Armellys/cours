#include "image.h"
#include "attacks.h"
#include "dct.h"
#include "mtrand.h"

int main(int argc, char** argv)
{
	if(argc!=3)
	{
		cout << argv[0] << " a.pgm b.pgm\n"; 
		cout << "\t a.pgm        : image A\n"; 
		cout << "\t b.pgm        : image B\n\n"; 
		
		return -1; 
	}
	
	image<octet>	a, b;
	
	long n = strtol(argv[2], NULL, 10); 
	
	
	a.read(argv[1]); 
	b.read(argv[2]); 

	double eqm = 0;
	
	for(int i=0; i<a.size(); i++)
	{
		eqm += (a[i] - b[i])*(a[i] - b[i]); 
	}


	cout << "PSNR = " << 10.0*log(255.0*255.0/(eqm/a.size()))/log(10.0) << endl; 

	return 1; 
}