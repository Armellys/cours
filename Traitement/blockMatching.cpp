#include <opencv2/opencv.hpp>
#include "opencv2/imgproc/imgproc.hpp"
#include <stdlib.h>
#include <highgui.h> 
#include <stdio.h>
#include <vector>
#include <math.h>

using namespace std;
using namespace cv;


main(int argc, char *argv[])
{
	
	Mat img;
	img = imread("similar-circles.jpg", 0); 
	if (!img.data){
		printf("No image Data");
		return -1;
	}

	
		
	waitKey(0);
	return 0;
}