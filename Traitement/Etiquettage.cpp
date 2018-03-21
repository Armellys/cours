#include <opencv2/opencv.hpp>
#include "opencv2/imgproc/imgproc.hpp"
#include <stdlib.h>
#include <highgui.h> 
#include <stdio.h>
#include <vector>
#include <math.h>

using namespace std;
using namespace cv;

int main(int argc, char *argv[])
{
	
	Mat img;
	img = imread("similar-circles.jpg", 0); 

	if (!img.data){
		printf("No image Data");
		return -1;
	}

	Mat imEnd = Mat(img.rows,img.cols,CV_8UC1,b);
	for (int i = 0; i < count; ++i)
	{
		for (int i = 0; i < count; ++i)
		{
			/* code */
		}
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