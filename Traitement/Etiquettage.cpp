#include <opencv2/opencv.hpp>
#include "opencv2/imgproc/imgproc.hpp"
#include <stdlib.h>
#include <highgui.h> 
#include <stdio.h>
#include <vector>
#include <math.h>

using namespace std;
using namespace cv;

void fusion (int i, int j, Mat& imEnd, int etiq);

void test(int x,int y, Mat& imEnd,int etiq){
	
	if( x>=0 && y<imEnd.cols && y>=0 && imEnd.at<uchar>(x,y)<etiq && imEnd.at<uchar>(x,y)!=0){
			fusion(x,y,imEnd,etiq);	
	}
}

void fusion (int i, int j, Mat& imEnd, int etiq){
	imEnd.at<uchar>(i,j) = etiq;
	int y= j-1;
	int x = i;
	test(x,y,imEnd,etiq);
	y= j-1;
	x = i-1;
	test(x,y,imEnd,etiq);
	y= j;
	x = i-1;
	test(x,y,imEnd,etiq);
	y= j+1;
	x = i-1;
	test(x,y,imEnd,etiq);
	
}

void nombreEtiquette ( Mat imEnd, int& etiq, int& n, int i, int j){
	//cout << "|";
	if(j-1>=0 && imEnd.at<uchar>(i,j-1)> 0){/*cout << " (1)";*/etiq = imEnd.at<uchar>(i,j-1); n++;}
	if(j-1>=0 && i-1>=0 && imEnd.at<uchar>(i-1,j-1)> 0){
		//cout << "(2)" ;
		n++;
		if(imEnd.at<uchar>(i-1,j-1)< etiq){ etiq = imEnd.at<uchar>(i-1,j-1);}
		
	} 
	if(i-1>=0 && imEnd.at<uchar>(i-1,j)> 0){
		n++;//cout << "(3)";
		if(imEnd.at<uchar>(i-1,j)< etiq) {etiq = imEnd.at<uchar>(i-1,j);}
		
	}
	if(j+1<imEnd.cols && i-1>=0 && imEnd.at<uchar>(i-1,j+1)> 0){
		n++;//cout << "(4)";
		if(imEnd.at<uchar>(i-1,j+1)< etiq){etiq = imEnd.at<uchar>(i-1,j+1);}
		
	}
	
}

int main(int argc, char *argv[])
{
	
	Mat img;
	img = imread("rice.png", 0); 
	if (!img.data){
		printf("No image Data");
		return -1;
	}
	int etiquette = 1;
	Mat imEnd = Mat(img.rows,img.cols,CV_8UC1);
	for (int i = 0; i < img.rows; ++i)
	{
		for (int j = 0; j <img.cols ; ++j)
		{
			imEnd.at<uchar>(i,j) = 0;
		}
	}

	imshow("End",imEnd);

	for (int i = 0; i < img.rows; ++i)
	{
		for (int j = 0; j < img.cols; ++j)
		{
			
			if (img.at<uchar>(i,j)==255){//si pixel blanc (au 1er plan)
				int etiq=1000; 
				int n = 0;
				nombreEtiquette(imEnd,etiq,n,i,j);
				//cout << " " << n << " " << etiq;
				if(n == 0){
					imEnd.at<uchar>(i,j)=(etiquette)%255;
					if (imEnd.at<uchar>(i,j)==0) imEnd.at<uchar>(i,j)++;
					etiquette++;
				}
				else if (n==1){
					imEnd.at<uchar>(i,j) = etiq;
				}else{
					//imEnd.at<uchar>(i,j) = etiq;
					fusion(i,j,imEnd,etiq);	
				}

			}

		}
		//cout << endl;
	}
	cout << "Nombre d'étiquette : " << etiquette << endl;
	
	imshow("riz",img);

	imshow("Etiquettage",imEnd);


	
		
	waitKey(0);
	return 0;
}