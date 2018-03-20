#include <opencv2/opencv.hpp>
#include "opencv2/imgproc/imgproc.hpp"
#include <stdlib.h>
#include <highgui.h> 
#include <stdio.h>
#include <vector>
#include <math.h>

#define SEUIL 10

using namespace std;
using namespace cv;

int norme(int pix, int moy){
	
	return 0;
}

int moyenne (list l, Mat img){

	return moy;
}
int croissance (int seed[2], Mat& imEnd, Mat img, int& somme, int& n){

	imEnd.at<uchar>(seed[0],seed[1]) = 255;
	int tab[2] = {seed[0]+1,seed[1]};
	
	int n = norme(img.at<uchar>(tab[0],tab[1]),moy);
	if(n<SEUIL){
		region.push_back(tab);

	}
	return 0;
}

int main(int argc, char *argv[])
{
	
	Mat im;
	im = imread("l.png", 0); // CV_LOAD_IMAGE_COLOR = 1
	int b[512][512]={0};
	Mat imEnd = Mat(512,512,CV_8UC1,b);

	if (!im.data){
		printf("No image Data");
		return -1;
	}
	int seed[2]={50,50};
	moy=img.at<uchar>(seed[0],seed[1])
	// x et y de la seed :
	
	


	
		
	waitKey(0);
	return 0;
}