#include <opencv2/opencv.hpp>
#include "opencv2/imgproc/imgproc.hpp"
#include <stdlib.h>
#include <highgui.h> 
#include <stdio.h>

using namespace std;
using namespace cv;

Mat myimconv(Mat im, Mat masque){

	Mat imConv = Mat(im.size().height,im.size().width, CV_32FC1) ;

	for (int k = (int)(masque.size().height/2) ; k<im.size().height- (int)(masque.size().height/2) ; k++){
	for (int l = (int)(masque.size().width/2); l< im.size().width -(int)(masque.size().width/2)  ; l++){
		float pixel = 0.0;
		for (int i = -(int)(masque.size().height/2) ; i<(int)(masque.size().height/2) ; i++){

			for (int j = -(int)(masque.size().width/2); j<(int)(masque.size().width/2); j++){
				pixel = pixel + masque.at<float>(i,j)*(float)im.at<uchar>(k-i,l-j);
				cout << (float)im.at<uchar>(k-i,l-j)* masque.at<float>(i,j) << endl;	
			}

		}
		
		imConv.at<float>(k,l) = pixel;
	}
	}
	
	double min, max;
	minMaxLoc(imConv,&min,&max);
	Mat imC; 
	imConv.convertTo(imC,CV_8U, 255.0/(max-min),-min*255.0/(max-min));

	return imC;
}

int main(int argc, char *argv[])
{
	
	Mat im;
	im = imread("index.jpeg", 0); // CV_LOAD_IMAGE_COLOR = 1

	if (!im.data){
		printf("No image Data");
		return -1;
	}
	
	Mat Masque = (Mat_<float>(5,5) << 1/9, 1/9, 1/9, 1/9, 1/9, 1/9, 1/9, 1/9, 1/9);
	cout << "Masque = " << endl << " " << Masque << endl << endl;
	
	Mat imC = myimconv(im,Masque);
	
	

	//affichage de l'image
	namedWindow("MonImageC", WINDOW_AUTOSIZE);	// containeur
	imshow("MonImageC",imC);				// affichage de l'image im
	namedWindow("MonImage", WINDOW_AUTOSIZE);
	imshow("MonImage",im);
	
	waitKey(0);
	return 0;
}
