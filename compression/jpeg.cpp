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
Mat Bloc (Mat im, int b, int nbBlocLigne){// b commence à 0 et correspond à notre num de bloc
	Mat result = Mat(SIZE,SIZE,CV_32FC1);
	int debuti = (SIZE*(b/nbBlocLigne))%im.rows;
	int debutj = (SIZE*b)%im.cols;
	for (int i = debuti; i < debuti+SIZE; ++i)
	{
		for (int j = debutj; j < debutj+SIZE; ++j)
		{
			result.at<float>(i%SIZE,j%SIZE) = (float)im.at<uchar>(i,j);
		}
	}

	return result;
}

//renvoie un tableau qui contient tous les bloc 8*8
void tabBloc (Mat image, Mat tabIm[],int nbBloc){

	for (int i = 0; i < nbBloc; ++i)
	{
		Mat monBloc=Mat(SIZE,SIZE,CV_32FC1);
		monBloc = Bloc(image,i,image.cols/SIZE);
		tabIm[i] = monBloc;
		
	}

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

void RLE (int bloc[SIZE*SIZE], fstream& fichier){
	int a=0;
	for (int i = 0; i < SIZE*SIZE; ++i)
	{
		
		if(bloc[i] != 0 ){
			if (a!=0){
				fichier << a;
				fichier<<"[0], ";
				a=0;
			}
			//char b = bloc[i]-128;
			fichier << bloc[i];
			//fichier << b;
			if (i != SIZE*SIZE -1) fichier << ", ";
		}
		else{
			a++;
			if(i == SIZE*SIZE -1){
				fichier << a;
				fichier << "[0]";
			}
		}
	}
	fichier <<endl;
}


int main(int argc, char *argv[])
{
	Mat image = imread("l.png", 0);
	int nbBloc = (image.cols/SIZE) * (image.rows/SIZE);

	Mat tabIm[nbBloc]; // contiendra les blocs de l'image
	Mat tabDct[nbBloc]; //contiendra la dct des blocs
	int tabU[nbBloc][SIZE][SIZE]; //contendra les résultat de quantification pour chaque bloc
	int tabQ[SIZE][SIZE];//contiendra le tableau de quantification
	int zig[nbBloc][SIZE*SIZE]; // cotient le parcours en zigzag

	getQuantification("quantification.txt",tabQ);// récupération du tableau de quantification
	fstream fichier("rle.txt",fstream::out | fstream::trunc);//ouverture du fichier rle

	tabBloc(image,tabIm,nbBloc); // met tous les blocs dans le tableau tabIm
	
	for (int i = 0; i < nbBloc; ++i)// pour chaque bloc
	{	
		dct(tabIm[i]-128,tabDct[i]);//dct
		divise(tabDct[i],tabQ,tabU[i]);	//quantification
		zigzag(tabU[i],zig[i]); // parcours en zigzag
		RLE(zig[i],fichier); // met le code RLE dans le fichier rle.txt

	}


	system("./huffman c rle.txt monjpeg.jpeg"); // on passe notre code rle à Huffman


	
	/*Mat dctBloc=Mat(8,8,CV_32FC1);
	dct(monBloc,dctBloc);*/

	/*Mat result = inverseBloc(tabIm, image, image.cols/8);
	Mat dct = inverseBloc(tabDct, image, image.cols/8);
	normalize(result,result, 0, 1, NORM_MINMAX);
	normalize(dct,dct, 0, 1, NORM_MINMAX);
	

	imshow( "mon image", dct );
	waitKey(0);*/
	//cout << nbBloc << endl << endl;
	/*Mat fimage;
	image.convertTo(fimage, CV_32F, 1.0/255);*/
		/*imshow( "DCT image", dctBloc );
		waitKey(0);*/

}