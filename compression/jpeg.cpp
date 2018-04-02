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

void divise(Mat bloc, int tab[SIZE][SIZE], int tabRes[SIZE][SIZE]){// divise une matrice par une autre, le resultat est dans tabRes 
	for (int i = 0; i < bloc.rows; ++i)
	{
		for (int j = 0; j < bloc.cols; ++j)
		{
			tabRes[i][j] = (int) round(bloc.at<float>(i,j)/tab[i][j]);
		}
	}

}

void getQuantification(string f,int tabQ[SIZE][SIZE]){ //récupération d'une matrice de quantification dans un fichier

	ifstream fichier(f,ios::in);  // on ouvre le fichier en lecture

        if(fichier)  
        {    
			for(int i = 0; i<SIZE; i++){
				for(int j = 0; j<SIZE; j++){
					fichier >> tabQ[i][j]; // lecture depuis le fichier 
				}
			}  
                fichier.close();  
        }

        else  
                cerr << "Impossible d'ouvrir le fichier !" << endl;

}

void zigzag(int bloc[SIZE][SIZE], int tab[SIZE*SIZE]){ /// lecture en zigzag d'un tableau en 2 dimensions
	
	tab[0] = bloc[0][0];
	int i=0,j=0;
	int a=0;
	bool b = false;
	while ( i<(SIZE-1) || j<(SIZE-1)){
		if (i == 0 || i == (SIZE-1)){// changement de diagonal haut
			j++;
			a++;
			if(i!=(SIZE-1)) b=true;
			else b=false;
			tab[a] = bloc[i][j];
		}
		else if (j == 0 || j == (SIZE-1)){// changement de diagonal bas
			i++;
			a++;
			if(j!=(SIZE-1)) b=false;
			else b=true;
			tab[a] = bloc[i][j];
			
		}
			if(b){
				while(j!=0 && i != (SIZE-1)){ // diagonal descendante
				j--;
				i++;
				a++;
				tab[a]=bloc[i][j];
				}
			}
			else{
				while(i!=0 && j != (SIZE-1)){ // diaginal ascendante
				i--;
				j++;
				a++;
				tab[a]= bloc[i][j];
				}
			}	
	}

}

void RLE (int bloc[SIZE*SIZE], fstream& fichier){ 
	char a=1, prev=bloc[0];// on garde le bloc précédent dans prev, a représente le nombre d'occurence d'un nombre
	for (int i = 1; i <= SIZE*SIZE; ++i)
	{
		if(i!=SIZE*SIZE && prev == bloc[i]) a++;// si le bloc courant est le même que celui d'avant
		else{
			if(a==1) {fichier << prev;} // si le nombre n'est apparu qu'une fois
			else if (a==2){fichier << prev<<prev;}// ou deux fois on réécrit juste le nombre ce nombre de fois
			else {fichier << a << "@" << prev;}// à partir de 3 fois on applique RLE, on met "nombre de fois@ le nombre" 	
			a=1;
		}
		prev = bloc[i];// sauvegarde du bloc précedent
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
		if ( i == 0){
			cout << tabU[0][0] << endl;
		}
		divise(blocDCT,tabQ,tabU);	//quantification
		if ( i == 0){
			cout << tabU[0][0] << endl;
		}
		zigzag(tabU,zig); // parcours en zigzag
		RLE(zig,fichier); // met le code RLE dans le fichier rle.txt

	}
	system("./huffman c rle.txt monjpeg.jpg"); // on passe notre code rle à Huffman
	return 0;
}