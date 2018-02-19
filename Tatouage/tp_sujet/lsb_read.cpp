/***********************

Adèle Journée

TP de Tatouage 

4A STI INSA CVL 2018


***********************/


#include "image.h"
#include "mtrand.h"

int main(int argc, char** argv)
{
	if(argc!=3)
	{
		cout << argv[0] << "src.pgm dst.pgm\n"; 
		cout << "\t src.pnm  : image marquée\n"; 
		cout << "\t dst.pgm  : image lue\n\n"; 
		
		return -1; 
	}
	
	mtsrand(78425UL);
	
	image<rgb> 	src; 
	image<octet>	dst;
	
	src.read(argv[1]); 

	dst.resize(src.width(), src.height()); 
	
	//Boucle pour parcourir notre image
	for(int y=0; y<src.height(); y++)
	{
		for(int x=0; x<src.width(); x++)
		{
			// on prend en compte notre générateur pseudo aléatoire
			if(mtrand()%2)// Si notre numéro aléatoire est paire on inverse
			{
				if(src(x,y).g%2){// si le pixel de l'image source est un 0

					dst(x,y)= 255; // on met un 1 dans la destination
				}
				else{
					dst(x,y)= 0; // sinon on met un 0 dans la destination
				}

			}
			else // si notre numéro aléatoire est impaire on laisse
			{
				if(src(x,y).g%2){// si le pixel de l'image source est un 0

					dst(x,y)= 0; //on met un 0 dans la destination
				}
				else{
					dst(x,y)= 255; // sinon met un 1 dans la destination
				}
			}
		}
	}
	
	
	
	
	dst.write(argv[2]); 
	
	return 1; 
}
