/***********************

Adèle Journée

TP de Tatouage 

4A STI INSA CVL 2018


***********************/

#include "dct.h"
#include "image.h"
#include "mtrand.h"

#include <vector>

#define N 		32 
#define NDCT	12
//#define ALPHA 	0.5 

// Les coefficients DCT qui seront marqués 
static unsigned int C[NDCT] = {1, 2, 3, 4, 8, 9, 10, 11, 16, 17, 18, 24};


int main(int argc, char** argv)
{
	if(argc!=4)
	{
		cout << argv[0] << " src.pgm dst.pgm 12345 \n"; 
		cout << "\t src.pgm : image hôte\n"; 
		cout << "\t dst.pgm : image marquée\n"; 
		cout << "\t 12345   : message (32 bits)\n\n"; 
		
		return -1; 
	}
	//génération du ALPHA
	float PSNR = 40.0;

	float ALPHA = sqrt((255*255*64)/(12*N*pow(10.0,PSNR/10)));
	cout<< ALPHA << endl;

	long message = strtol(argv[3], NULL, 10); 
	
	// Génération du message 
	vector<int> M(N); for(int i=0; i<N; i++) M[i] = (message>>i)%2;
	
	mtsrand(78425UL);
	
	image<octet> 		src, dst; 
	image<double>	src_dct; 
	image<double>	src_watson;
	
	src.read(argv[1]); 
	
	// Transformée DCT de l'image hôte
	dct::analyse(src, src_dct); 
	dct::watson(src_dct, src_watson);
	
	// Génération du vecteur hôte 
	vector<double*> X(NDCT*src_dct.size()/64); int cur(0); 
	vector<double*> S(NDCT*src_dct.size()/64);// vecteur pour watson
	double moy_watson=0;

	for(int by=0; by<src.height()/8; by++)
	{
		for(int bx=0; bx<src.width()/8; bx++)
		{
			for(int i=0; i<NDCT; i++)
			{
				const unsigned int x  = 8*bx + C[i]%8;  
        		const unsigned int y  = 8*by + C[i]/8;
        
				X[cur]=&(src_dct(x, y));
				S[cur]=&(src_watson(x, y)); 

				moy_watson += *S[cur];
				cur++;
			}
		}
	}
	moy_watson = moy_watson/S.size();

	// Tatouage 
	// Todo 

	// Génération de la matrice G (obsolète)
	/*int G[N][X.size()];
	for(int i=0; i<N; i++)
	{
		for(int j=0; j<X.size(); j++)
		{
			if(mtrand()%2){
				G[i][j]=1;
			}
			else{
				G[i][j]=-1;
			}
		}
	}*/
	// on remplace les 0 par -1
	for(int j=0; j<N; j++)
	{
		if(M[j]==0){
			M[j]=-1;
		}
		
	}
	// on itère 
	for(int i=0; i<X.size(); i++)
	{
		int s =0;

		for(int j=0; j<N; j++)
		{
			if(mtrand()%2){
				s+=M[j];// ici la matrice est à 1
			}
			else{
				s+=M[j]*(-1);// et ici la matrice est à -1
			}
		}
		int w = *(S[i])*ALPHA*s/(moy_watson*sqrt(2));// on multiplie la somme par ALPHA et pondère par watson
		*(X[i])=*(X[i])+w;// puis on tatoue l'image
	}


	// Transformée inverse 
	dct::synthese(src_dct, dst); 

	// Calcul du PSNR
	double eqm(0); for(int i=0; i<dst.size(); i++) eqm+=(dst[i] - src[i])*(dst[i] - src[i]); 
	cout << "EQM = " << eqm/dst.size() << " | PSNR = " << log(65025.0/eqm*dst.size())/0.23026 << " dB\n"; 

	// Sauvegarde de l'image marquée 
	dst.write(argv[2]); 
	
	return 1; 
}