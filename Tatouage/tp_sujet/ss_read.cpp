#include "dct.h"
#include "image.h"
#include "mtrand.h"

#include <vector>

#define N 		32 
#define NDCT	12
#define ALPHA 	0.5 

// Les coefficients DCT qui seront marqués 
static unsigned int C[NDCT] = {1, 2, 3, 4, 8, 9, 10, 11, 16, 17, 18, 24};


int main(int argc, char** argv)
{
	if(argc!=2)
	{
		cout << argv[0] << " src.pgm\n"; 
		cout << "\t src.pgm : image marquée\n\n"; 
		
		return -1; 
	}
	
	// Message 
	vector<double> M(N, 0); 
	
	mtsrand(78425UL);
	
	image<octet> 		src; 
	image<double>	src_dct; 
	
	src.read(argv[1]); 
	
	// Transformée DCT de l'image hôte
	dct::analyse(src, src_dct); 
	
	// Génération du vecteur hôte 
	vector<double*> X(NDCT*src_dct.size()/64); int cur(0); 

	for(int by=0; by<src.height()/8; by++)
	{
		for(int bx=0; bx<src.width()/8; bx++)
		{
			for(int i=0; i<NDCT; i++)
			{
				const unsigned int x  = 8*bx + C[i]%8;  
        		const unsigned int y  = 8*by + C[i]/8;
        
				X[cur++]=&(src_dct(x, y)); 
			}
		}
	}



	// Lecture du tatouage 	
	// Todo 
	
	

	// Affichage du message 
	long message(0); for(int i=0; i<N; i++) if(M[i]>0) message+=1<<i; 
		
	cout << "Message : " << message << endl; 
	
	// Quelques statistiques sur le canal 
	double var(0), moy(0); 
	
	for(int i=0; i<N; i++) if(M[i]>0) moy+=M[i]; else moy-=M[i]; 
	for(int i=0; i<N; i++) if(M[i]>0) var+=CARRE(M[i] - moy/N); else var+=CARRE(M[i] + moy/N); 
	
	cout << "SNR = " << CARRE(moy/N) << " / " << var/N << " = " << CARRE(moy/N)/(var/N) << endl; 
	
	return 1; 
}