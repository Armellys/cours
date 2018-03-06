#include <opencv2/opencv.hpp>
#include "opencv2/imgproc/imgproc.hpp"
#include <stdlib.h>
#include <highgui.h> 
#include <stdio.h>
#include <vector>

using namespace std;
using namespace cv;

Mat myimconv(Mat im, Mat masque){

	Mat imConv = Mat(im.size().height,im.size().width, CV_32FC1) ;
	// double for pour parcourir l'image
	for (int k = (int)(masque.size().height/2) ; k<im.size().height- (int)(masque.size().height/2) ; k++){
	for (int l = (int)(masque.size().width/2); l< im.size().width -(int)(masque.size().width/2)  ; l++){
		float pixel = 0.0;
		for (int i = -(int)(masque.size().height/2) ; i<=(int)(masque.size().height/2) ; i++){// on parcours le masque
			for (int j = -(int)(masque.size().width/2); j<=(int)(masque.size().width/2); j++){
				pixel = pixel + masque.at<float>(i,j)*(float)im.at<uchar>(k-i,l-j);
				//cout << (float)im.at<uchar>(k-i,l-j)* masque.at<float>(i,j) << endl;	
			}
		}
		//cout << pixel << endl;
		imConv.at<float>(k,l) = pixel;
	}
	}
	
	double min, max;
	minMaxLoc(imConv,&min,&max);
	Mat imC; 

	//imConv.convertTo(imC,CV_32FC1, 1.0/abs(max-min),-min*1.0/abs(max-min)); // convertit en float entre 0 et 1
	imConv.convertTo(imC,CV_8UC1, 255.0/abs(max-min),min*255.0/abs(max-min));

	return imC;
}

Mat myimFiltreMed(Mat im, Mat masque){

	Mat imFiltre = Mat(im.size().height,im.size().width, CV_32FC1) ;

	for (int k = (int)(masque.size().height/2) ; k<im.size().height- (int)(masque.size().height/2) ; k++){
	for (int l = (int)(masque.size().width/2); l< im.size().width -(int)(masque.size().width/2)  ; l++){
		vector<uchar> pixel(masque.size().height*masque.size().width);
		int h = 0;
		for (int i = -(int)(masque.size().height/2) ; i<=(int)(masque.size().height/2) ; i++){//lon parcours le masque de -1 à 1

			for (int j = -(int)(masque.size().width/2); j<=(int)(masque.size().width/2); j++){
				pixel[h]= im.at<uchar>(k-i,l-j);
				h++;	
				
			}	

		}
		
		sort(pixel.begin(),pixel.end());
		int milieu = masque.size().height*masque.size().width/2;
		imFiltre.at<float>(k,l) = pixel[milieu];
	}
	}
	
	double min, max;
	minMaxLoc(imFiltre,&min,&max);
	Mat imC; 
	imFiltre.convertTo(imC,CV_8UC1, 255.0/abs(max-min),-min*255.0/abs(max-min)); // convertit en float entre 0 et 1

	return imC;
}



int main(int argc, char *argv[])
{
	
	Mat im;
	im = imread("LennaBruit.png", 0); // CV_LOAD_IMAGE_COLOR = 1

	if (!im.data){
		printf("No image Data");
		return -1;
	}
	
	Mat Masque = (Mat_<float>(3,3) << 1/9, 1/9, 1/9, 1/9, 1/9, 1/9, 1/9, 1/9, 1/9);
	cout << "Masque = " << endl << " " << Masque << endl << endl;
	//cout << "taille : " << Masque.size().height;
	Mat imC = myimconv(im,Masque);
	Mat imF =myimFiltreMed(im,Masque);
	

	//affichage de l'image
	namedWindow("MonImageConvolution", WINDOW_AUTOSIZE);	// containeur
	imshow("MonImageConvolution",imC);				// affichage de l'image im
	namedWindow("MonImageBase", WINDOW_AUTOSIZE);
	imshow("MonImageBase",im);
	namedWindow("MonImageFiltrée", WINDOW_AUTOSIZE);
	imshow("MonImageFiltrée",imF);
	waitKey(0);
	return 0;
}
