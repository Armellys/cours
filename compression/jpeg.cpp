// g++ helloworldCV.cpp `pkg-config --cflags --libs opencv`

#include <opencv2/opencv.hpp>
#include "opencv2/imgproc/imgproc.hpp"
#include <stdlib.h>
#include <highgui.h> 
#include <stdio.h>
#include <vector>
#include <math.h>
#include <string>
#include <fstream>
#include <utility>

#define SIZE 8 //taille des blocs SIZE*SIZE

using namespace std;
using namespace cv;



//découpe en bloc de 8 par 8
Mat Bloc (Mat im, int b){// b commence à 0 et correspond à notre numero de bloc
	Mat result = Mat(SIZE,SIZE,CV_32FC1);
	int debuti = (SIZE*(b/(im.cols/SIZE)))%im.rows; // calcul de l'indice de début pour la ligne
	int debutj = (SIZE*b)%im.cols;// calcul de l'indice de début pour les colonnes
	// boucle de parcours
	for (int i = debuti; i < debuti+SIZE; ++i)
	{
		for (int j = debutj; j < debutj+SIZE; ++j)
		{
			result.at<float>(i%SIZE,j%SIZE) = (float)im.at<uchar>(i,j);
		}
	}

	return result;
}

Mat inverseBloc (Mat tabIm[], Mat image, int nbBlocLigne){// remet le tableau de bloc en une seule image
    
	Mat result = Mat(image.rows,image.cols,CV_32FC1);
	int nbBloc = (image.cols/SIZE) * (image.rows/SIZE);
	for (int b = 0; b < nbBloc; b++)
	{
		int debuti = (SIZE*(b/nbBlocLigne))%image.rows;
		int debutj = (SIZE*b)%image.cols;
		for (int i = debuti; i < debuti+SIZE; ++i)
		{
			for (int j = debutj; j < debutj+SIZE; ++j)
			{
				result.at<float>(i,j) = tabIm[b].at<float>(i%SIZE,j%SIZE);
			}
		}
	}

	return result;

}

void divise(Mat bloc, int tab[SIZE][SIZE], int tabRes[SIZE][SIZE]){// remet le tableau de bloc en une seule image
    
	
	for (int i = 0; i < bloc.rows; ++i)
	{
		for (int j = 0; j < bloc.cols; ++j)
		{
			tabRes[i][j] = (int) round(bloc.at<float>(i,j)/tab[i][j]);
		}
	}

}

void getQuantification(string f,int tabQ[SIZE][SIZE]){

	ifstream fichier(f,ios::in);  // on ouvre le fichier en lecture

        if(fichier)  
        {    
		for(int i = 0; i<SIZE; i++){
			for(int j = 0; j<SIZE; j++){
				fichier >> tabQ[i][j];
			}  
			cout << endl;
		}  
                fichier.close();  
        }

        else  
                cerr << "Impossible d'ouvrir le fichier !" << endl;

}

void zigzag(int bloc[SIZE][SIZE], int tab[SIZE*SIZE]){
	
	tab[0] = bloc[0][0];
	int i=0,j=0;
	int a=0;
	bool b = false;
	while ( i<(SIZE-1) || j<(SIZE-1)){
		if (i == 0 || i == (SIZE-1)){
			j++;
			a++;
			if(i!=(SIZE-1)) b=true;
			else b=false;
			tab[a] = bloc[i][j];
		}
		else if (j == 0 || j == (SIZE-1)){
			i++;
			a++;
			if(j!=(SIZE-1)) b=false;
			else b=true;
			tab[a] = bloc[i][j];
			
		}
			if(b){
				while(j!=0 && i != (SIZE-1)){
				j--;
				i++;
				a++;
				tab[a]=bloc[i][j];
				}
			}
			else{
				while(i!=0 && j != (SIZE-1)){
				i--;
				j++;
				a++;
				tab[a]= bloc[i][j];
				}
			}	
	}

}

void RLE (int bloc[SIZE*SIZE], fstream& fichier){ // faire pour tous les symbole et mettre un @
	int a=1, prev=bloc[0];
	for (int i = 1; i <= SIZE*SIZE; ++i)
	{
		if(i!=SIZE*SIZE && prev == bloc[i]) a++;
		else{
			if(a==1) {fichier << prev << " ";}
			else {fichier << a << "@" << prev << " ";}	
			a=1;
		}
		prev = bloc[i];
	
	}
	fichier <<endl;
}


int main(int argc, char *argv[])
{
	if ( argc < 2 ) {
		cout << "Usage : ./jpeg [photo non compréssée]" << endl;
		cout << "Exemple : ./jpeg l.png " << endl;
		cout << "La sortie se trouvera dans \" monjpeg.jpg \"" << endl;
		return 0;
	}
	Mat image = imread(argv[1], 0);
	int nbBloc = (image.cols/SIZE) * (image.rows/SIZE);

	Mat tabIm[nbBloc]; // contiendra les blocs de l'image
	Mat blocDCT; //contiendra la dct des blocs
	int tabU[SIZE][SIZE]; //contendra les résultat de quantification pour chaque bloc
	int tabQ[SIZE][SIZE];//contiendra le tableau de quantification
	int zig[SIZE*SIZE]; // contient le parcours en zigzag

	getQuantification("quantification.txt",tabQ);// récupération du tableau de quantification
	fstream fichier("rle.txt",fstream::out | fstream::trunc);//ouverture du fichier rle
	
	for (int i = 0; i < nbBloc; ++i)// pour chaque bloc
	{	Mat monBloc = Bloc(image,i);// calcul du Ieme bloc
		dct(monBloc-128,blocDCT);//dct
		divise(blocDCT,tabQ,tabU);	//quantification
		zigzag(tabU,zig); // parcours en zigzag
		RLE(zig,fichier); // met le code RLE dans le fichier rle.txt

	}


	system("./huffman c rle.txt monjpeg.jpg"); // on passe notre code rle à Huffman
	return 0;


}