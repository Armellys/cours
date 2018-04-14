#include <opencv2/opencv.hpp>
#include "opencv2/imgproc/imgproc.hpp"
#include <stdlib.h>
#include <highgui.h> 
#include <stdio.h>
#include <vector>
#include <math.h>

using namespace std;
using namespace cv;

void fusion (int i, int j, Mat& imEnd, int etiq, bool& fus);

void test(int x,int y, Mat& imEnd,int etiq, bool& fus){
	
	if( x>=0 && y<imEnd.cols && y>=0 && imEnd.at<uchar>(x,y)>etiq && imEnd.at<uchar>(x,y)!=0){
			fus = true;
			fusion(x,y,imEnd,etiq,fus);	
	}
}

void fusion (int i, int j, Mat& imEnd, int etiq,bool& fus){
	imEnd.at<uchar>(i,j) = etiq;
	int y= j-1;
	int x = i;
	test(x,y,imEnd,etiq,fus);
	y= j-1;
	x = i-1;
	test(x,y,imEnd,etiq,fus);
	y= j;
	x = i-1;
	test(x,y,imEnd,etiq,fus);
	y= j+1;
	x = i-1;
	test(x,y,imEnd,etiq,fus);
	
}

void nombreEtiquette ( Mat imEnd, int& etiq, int& n, int i, int j){
	
	if(j-1>=0 && imEnd.at<uchar>(i,j-1)> 0){etiq = imEnd.at<uchar>(i,j-1); n++;}
	if(j-1>=0 && i-1>=0 && imEnd.at<uchar>(i-1,j-1)> 0){;
		n++;
		if(imEnd.at<uchar>(i-1,j-1)< etiq){ etiq = imEnd.at<uchar>(i-1,j-1);}
	} 
	if(i-1>=0 && imEnd.at<uchar>(i-1,j)> 0){
		n++;
		if(imEnd.at<uchar>(i-1,j)< etiq) {etiq = imEnd.at<uchar>(i-1,j);}
		
	}
	if(j+1<imEnd.cols && i-1>=0 && imEnd.at<uchar>(i-1,j+1)> 0){
		n++;
		if(imEnd.at<uchar>(i-1,j+1)< etiq){etiq = imEnd.at<uchar>(i-1,j+1);}
		
	}
	
}

void normalisation(Mat& im){
	for (int i = 0; i < im.rows; ++i)
	{
		for (int j = 0; j <im.cols ; ++j)
		{
			if(im.at<uchar>(i,j) > 128) im.at<uchar>(i,j) = 0;
			else im.at<uchar>(i,j) = 255;
		}
	}
	
}

int main(int argc, char *argv[])
{
	
	Mat img;
	img = imread("similar-circles.jpg", 0); 
	if (!img.data){
		printf("No image Data");
		return -1;
	}
	// on normalise l'image pour avoir une image que en noir et blanc (0 ou 255)
	normalisation(img);
	// on initialise notre première étiquette
	int etiquette = 0;
	// on initialise notre image de fin
	Mat imEnd = Mat(img.rows,img.cols,CV_8UC1);
	for (int i = 0; i < img.rows; ++i)
	{
		for (int j = 0; j <img.cols ; ++j)
		{
			imEnd.at<uchar>(i,j) = 0;
		}
	}

	// on va parcourir chaque pixel de notre image
	for (int i = 0; i < img.rows; ++i)
	{
		for (int j = 0; j < img.cols; ++j)
		{
			
			if (img.at<uchar>(i,j) >= 128){//si le pixel est blanc, donc qu'il est au premier plan (pas le fond)
				int etiq=1000; // etiquette provisoire
				int n = 0;// nombre d'étiquette dejà existante autour de notre pixel
				nombreEtiquette(imEnd,etiq,n,i,j);// nous renvoie le nombre d'étiquette deja présente autour de notre pixel dans n
				// et aussi l'étiquette la plus petite dans etiq s'il y a eu plusieur etiquette
				if(n == 0){// s'il n y a pas eu d'étiquette
					etiquette++; // on crée une nouvelle étiquette
					imEnd.at<uchar>(i,j)=(etiquette*10)%255;// on colorie notre pixel par rapport à l'étiquette
					if (imEnd.at<uchar>(i,j)==0) imEnd.at<uchar>(i,j)++;// si par hasard l'étiquette tombe sur 0, on incrémente pour ne pas confondre avec le fond qui est à 0 aussi					
				}
				else if (n==1){//si il n y a eu qu'une étiquette à cotès de n otre pixel
					imEnd.at<uchar>(i,j) = etiq;// on colorie notre pixel
				}else{// s'il y a eu plusieur étiquette
					bool fus = false;
					fusion(i,j,imEnd,etiq,fus);// on fusionne les étiquette	
					if(fus) etiquette--;// et si on a fusionner on décrémente le nombre d'étiquette car cela veut dire qu'une étiquette à disparu
				}

			}

		}
	}
	cout << "Nombre d'étiquette : " << etiquette << endl;
	
	imshow("cercles",img); // affichage de l'image en noir et blanc

	imshow("Etiquettage",imEnd); // affichage de notre etiquettage de l'image
		
	waitKey(0);
	return 0;
}