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
	//cout << "|";
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
	normalisation(img);
	int etiquette = 1;
	Mat imEnd = Mat(img.rows,img.cols,CV_8UC1);
	for (int i = 0; i < img.rows; ++i)
	{
		for (int j = 0; j <img.cols ; ++j)
		{
			imEnd.at<uchar>(i,j) = 0;
		}
	}
	for (int i = 0; i < img.rows; ++i)
	{
		for (int j = 0; j < img.cols; ++j)
		{
			
			if (img.at<uchar>(i,j) >= 128){//si pixel blanc (au 1er plan)
				int etiq=1000; 
				int n = 0;
				nombreEtiquette(imEnd,etiq,n,i,j);
				if(n == 0){
					imEnd.at<uchar>(i,j)=(etiquette*10)%255;
					if (imEnd.at<uchar>(i,j)==0) imEnd.at<uchar>(i,j)++;
					etiquette++;
				}
				else if (n==1){
					imEnd.at<uchar>(i,j) = etiq;
				}else{
					bool fus = false;
					fusion(i,j,imEnd,etiq,fus);	
					if(fus) etiquette--;
				}

			}

		}
	}
	cout << "Nombre d'étiquette : " << etiquette-1 << endl;
	
	imshow("cercles",img);

	imshow("Etiquettage",imEnd);
		
	waitKey(0);
	return 0;
}