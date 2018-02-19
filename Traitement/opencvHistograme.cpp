/*
#include <cv.h>
#include <highgui.h> //affichage

cv::Mat image; //déclaration d'image
im= cv::imread("img.jpg", CV_LOAD_IMAGE_COLOR);*/ //lire fichier image en format couleur

/************************** EQUIVALENT ***********************/
#include <opencv2/opencv.hpp>
#include <highgui.h> 
#include <stdio.h>

using namespace std;
using namespace cv;

int main(int argc, char *argv[])
{
	
	Mat im;
	im = imread("index.jpeg", CV_LOAD_IMAGE_COLOR); // CV_LOAD_IMAGE_COLOR = 1

	if (!im.data){
		printf("No image Data");
		return -1;
	}

	//affichage de l'image
	namedWindow("MonImage", WINDOW_AUTOSIZE);	// containeur
	imshow("MonImage",im);				// affichage de l'image im

	uchar a = (uchar) im.at<uchar>(10,10); // renvoi la valeur de la couleur du pixel à (10,10)
	uchar b = (uchar) im.at<Vec3b>(10,10)[1];
	uchar c = (uchar) im.at<Vec3b>(10,10)[2];
	uchar d = (uchar) im.at<Vec3b>(10,10)[0];
	cout << (int)a << endl;// affiche sur la sortie standard, endl pour le retour à la ligne
	cout << (int)b << endl;
	cout << (int)c << endl;
	cout << (int)d << endl;
	
	printf("Histogramme\n");
	im = imread("index.jpeg", 0);//en noir et blanc
	int tab[256];
	
	int i=0,j=0;

	for(i=0;i<256;i++){
		tab[i]=0;
	}

	for(i=0;i<im.size().height;i++){
		for(j=0;j<im.size().width;j++){
				
			tab[im.at<uchar>(i,j)]++;

		}
	}
	for(i=0;i<256;i++){
		for(j=0;j<tab[i];j++){
			printf("|");
		}
		printf("\n");
	}

	
	waitKey(0);
	return 0;
}
