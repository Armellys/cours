#include <opencv2/opencv.hpp>
#include "opencv2/imgproc/imgproc.hpp"
#include <stdlib.h>
#include <highgui.h> 
#include <stdio.h>
#include <vector>
#include <math.h>

#define SEUIL 45

using namespace std;
using namespace cv;

int croissance(int[2],Mat&,Mat,int&,int&);

int norme(int pix, int moy){
	
	return (pix-moy)*(pix-moy);
}
void test(Mat img,int tab[2],int& somme, int& n, Mat& imEnd){
	
	if((tab[0]<img.rows && tab[0]>=0) && (tab[1]<img.cols && tab[1]>=0) && imEnd.at<uchar>(tab[0],tab[1])!=255){
		int nrm = norme(img.at<uchar>(tab[0],tab[1]),somme/n);// calcul de la norme
		if(nrm<SEUIL){
			n++;
			somme+=img.at<uchar>(tab[0],tab[1]);
			croissance(tab, imEnd, img, somme,n);	
		}
	}
}

int croissance (int seed[2], Mat& imEnd, Mat img, int& somme, int& n){

	imEnd.at<uchar>(seed[0],seed[1]) = 255; // on colorie le pixel sur l'image 
	int tab[2] = {seed[0]+1,seed[1]}; // on regarde le pixel voisin
	test(img,tab,somme,n,imEnd);
	tab[0] = seed[0]-1;
	tab[1] = seed[1];
	test(img,tab,somme,n,imEnd);
	tab[0] = seed[0];
	tab[1] = seed[1]+1;
	test(img,tab,somme,n,imEnd);
	tab[0] = seed[0];
	tab[1] = seed[1]-1;
	test(img,tab,somme,n,imEnd);

	return 0;
}

int main(int argc, char *argv[])
{
	
	Mat img;
	img = imread("batiment.jpg", 0); // CV_LOAD_IMAGE_COLOR = 1
	int b[img.rows][img.cols]={0};
	Mat imEnd = Mat(img.rows,img.cols,CV_8UC1,b);

	if (!img.data){
		printf("No image Data");
		return -1;
	}
	int seed[2]={150,150};
	int somme=img.at<uchar>(seed[0],seed[1]);
	int n=1;
	cout << "coucou" << endl;
	croissance(seed, imEnd, img, somme,n);
	
	
	imshow("Croissance",imEnd);


	
		
	waitKey(0);
	return 0;
}