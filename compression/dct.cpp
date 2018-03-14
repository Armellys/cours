// g++ helloworldCV.cpp `pkg-config --cflags --libs opencv`

#include <opencv2/opencv.hpp>
#include "opencv2/imgproc/imgproc.hpp"
#include <stdlib.h>
#include <highgui.h> 
#include <stdio.h>
#include <vector>
#include <math.h>

using namespace std;
using namespace cv;


//découpe en bloc de 8 par 8
Mat Bloc (Mat im, int b, int nbBlocLigne){// b commence à 0 et correspond à notre num de bloc
	Mat result = Mat(8,8,CV_32FC1);
	int debuti = (8*(b/nbBlocLigne))%im.rows;
	int debutj = (8*b)%im.cols;
	for (int i = debuti; i < debuti+8; ++i)
	{
		for (int j = debutj; j < debutj+8; ++j)
		{
			result.at<float>(i%8,j%8) = (float)im.at<uchar>(i,j);
		}
	}

	return result;
}

//renvoie un tableau qui contient tous les bloc 8*8
void tabBloc (Mat image, Mat tabIm[],int nbBloc){

	for (int i = 0; i < nbBloc; ++i)
	{
		Mat monBloc=Mat(8,8,CV_32FC1);;
		monBloc = Bloc(image,i,image.cols/8);
		tabIm[i] = monBloc;
		
	}

}

Mat inverseBloc (Mat tabIm[], Mat image, int nbBlocLigne){// remet le tableau de bloc en une seule image
    
	Mat result = Mat(image.rows,image.cols,CV_32FC1);
	int nbBloc = (image.cols/8) * (image.rows/8);
	for (int b = 0; b < nbBloc; b++)
	{
		int debuti = (8*(b/nbBlocLigne))%image.rows;
		int debutj = (8*b)%image.cols;
		for (int i = debuti; i < debuti+8; ++i)
		{
			for (int j = debutj; j < debutj+8; ++j)
			{
				result.at<float>(i,j) = tabIm[b].at<float>(i%8,j%8);
			}
		}
	}

	return result;

}


int main(int argc, char *argv[])
{
	Mat image = imread("camion.pgm", 0);
	int nbBloc = (image.cols/8) * (image.rows/8);
	Mat tabIm[nbBloc];
	Mat tabDct[nbBloc];
	tabBloc(image,tabIm,nbBloc); // met tous les blocs dans le tableau tabIm

	for (int i = 0; i < nbBloc; ++i)
	{
		dct(tabIm[i],tabDct[i]);
	}

	Mat dctBloc=Mat(8,8,CV_32FC1);
	dct(monBloc,dctBloc);

	Mat result = inverseBloc(tabIm, image, image.cols/8);
	normalize(result,result, 0, 1, NORM_MINMAX);
	

	imshow( "mon image", result );
	waitKey(0);
	//cout << nbBloc << endl << endl;
	/*Mat fimage;
	image.convertTo(fimage, CV_32F, 1.0/255);*/
		/*imshow( "DCT image", dctBloc );
		waitKey(0);*/

}