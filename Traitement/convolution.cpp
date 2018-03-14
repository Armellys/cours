#include <opencv2/opencv.hpp>
#include "opencv2/imgproc/imgproc.hpp"
#include <stdlib.h>
#include <highgui.h> 
#include <stdio.h>
#include <vector>
#include <math.h>

using namespace std;
using namespace cv;
const int   kMaskRows  = 3;
const int   kMaskCols  = 3;
const float kMoyenneur = 10.0;

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
	//double min, max;
	//minMaxLoc(imConv,&min,&max);
	//Mat imC; 

	//imConv.convertTo(imC,CV_32FC1, 1.0/abs(max-min),-min*1.0/abs(max-min)); // convertit en float entre 0 et 1
	//imConv.convertTo(imC,CV_8UC1, 255.0/abs(max-min),min*255.0/abs(max-min));

	return imConv;
}

void calc_conv(Mat& src, Mat& dst, Mat& mask, int x, int y)
{	
	int dimMaskX = mask.size().width;
	int dimMaskY = mask.size().height;
	int offsetX  = dimMaskX/2;
	int offsetY  = dimMaskY/2;
		float res(0.0);
		for(int i=(-offsetX); i<(1+offsetX); i++)
		{
			for(int j=(-offsetY); j<(1+offsetY); j++)
			{
				res += (float)src.at<uchar>(x-i, y-j) * mask.at<float>(i+offsetX,j+offsetY);
			}
		}
		dst.at<float>(x,y) = res/kMoyenneur;
	
}

void normalize_float(Mat& src)
{	
		float max(0.0);
		// Recherche du max
		for(int x=0; x<src.size().width; x++)
	    {
	    	for(int y=0; y<src.size().height; y++)
	    	{
	    		if(src.at<float>(x,y)>max)
	    		{
	    			max = src.at<float>(x,y);
	    		}
	    	}
	    }
	    // Normalisation [0, 1]
	    for(int x=0; x<src.size().width; x++)
	    {
	    	for(int y=0; y<src.size().height; y++)
	    	{
			    src.at<float>(x,y) /= max;
	    	}
	    }
}

Mat convolution(Mat& src, Mat& mask)
{
	
	int dimX = src.size().width-1;
	int dimY = src.size().height-1;

	Mat result;
	result = Mat(dimX, dimY, CV_32FC1);
	
	for(int x=1; x<dimX; x++)
	{
		for(int y=1; y<dimY; y++)
		{
			calc_conv(src, result, mask, x, y);
		}
	}

	normalize_float(result);
	/*namedWindow("Convolution", WINDOW_AUTOSIZE );
    imshow("Convolution", result);*/
    return result;
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

Mat sqrtMat(Mat im){
	int dimX = im.size().width;
	int dimY = im.size().height;

	Mat result = Mat(im.size().height,im.size().width,CV_32FC1);;
	result = Mat(dimX, dimY, CV_32FC1);
	
	for(int x=1; x<dimX; x++)
	{
		for(int y=1; y<dimY; y++)
		{
			result.at<float>(x,y)= sqrt(im.at<float>(x,y));
		}
	}
	//normalize_float(result);
	return result;
}

Mat carreMat(Mat im){
	int dimX = im.size().width;
	int dimY = im.size().height;

	Mat result = Mat(im.size().height,im.size().width,CV_32FC1);
	result = Mat(dimX, dimY, CV_32FC1);
	
	for(int x=1; x<dimX; x++)
	{
		for(int y=1; y<dimY; y++)
		{
			result.at<float>(x,y)= im.at<float>(x,y)*im.at<float>(x,y);
		}
	}
	//normalize_float(result);
	return result;
}


float mulSum(Mat src, int i, int j, Mat mask){

	float sum = 0.0 ;
	for (int l =0   ; l<mask.size().height ; l++){
		for (int k =0  ; k<mask.size().width ; k++){
		     sum += (float)src.at<uchar>(i-l,j-k)*mask.at<float>(l,k);
		
		}
	}

	return sum ;
}

int myConv2D(Mat src, Mat mask, Mat dest){//fonction du prof

	int ihwin = (int)(mask.size().height/2) ;
	int jhwin = (int)(mask.size().width/2) ;

	int i, j;
	for (i = ihwin  ; i<src.size().height-ihwin ; i++){
		for (j = jhwin  ; j<src.size().width-jhwin ; j++){
	           dest.at<float>(i,j) = mulSum(src,i,j,mask);
	            
		}

	}

	return 0 ;
}



int main(int argc, char *argv[])
{
	
	Mat im;
	im = imread("l.png", 0); // CV_LOAD_IMAGE_COLOR = 1

	if (!im.data){
		printf("No image Data");
		return -1;
	}
	
	
	Mat Masque = (Mat_<float>(3,3) << 1/9, 1/9, 1/9,1/9, 1/9, 1/9, 1/9, 1/9, 1/9);
	
	float a[9]={-1.0,-2.0,-1.0,0.0,0.0,0.0,1.0,2.0,1.0};
    float b[9]={-1.0,0.0,1.0,-2.0,0.0,2.0,-1.0,0.0,1.0};
		
    Mat H1 = Mat(3,3,CV_32FC1,a);
    Mat H2 = Mat(3,3,CV_32FC1,b);

	Mat imF = myimFiltreMed(im,Masque);
	
	Mat imH1 = Mat(imF.size().height,imF.size().width,CV_32FC1);	
	myConv2D(imF,H1,imH1);

	Mat imH2 = Mat(imF.size().height,imF.size().width,CV_32FC1);
	myConv2D(imF,H2,imH2);

	/*normalize(imH1,imH1, 0, 1, NORM_MINMAX);
	normalize(imH2,imH2, 0, 1, NORM_MINMAX);*/

	Mat carre1 = carreMat(imH1);
	Mat carre2 = carreMat(imH2);
	Mat imC = sqrtMat(carre1+carre2);

	normalize(imC,imC, 0, 1, NORM_MINMAX);
	normalize(imH1,imH1, 0, 1, NORM_MINMAX);
	normalize(imH2,imH2, 0, 1, NORM_MINMAX);
	
	//affichage de l'image
	
	
		// containeur
	imshow("h1",imH1);	
	imshow("h2",imH2);
	imshow("MonImageContour",imC);				// affichage de l'image im
		
	waitKey(0);
	return 0;
}
