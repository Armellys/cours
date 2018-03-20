#include <stdio.h>
#include <cv.h>
#include <highgui.h>
#include <cxcore.h>
#include <iostream>
//A. HAfiane, INSA CVL
using namespace std ;
double *histo(IplImage *img){//IplImage ancien Mat
int i, j, hist[256]={0};
double *p = new double[256];

  for(i=0; i<img->height;i++)
     for(j=0;j<img->width;j++)
           hist[(int)cvGet2D(img,i,j).val[0]]++;
        			
		
   for(i=0;i<256;i++)
	p[i]=(double)hist[i]/(img->width*img->height)	;

return p;
}


double somme(double *p, int debut, int fin){
double s=0.0;
  for(int i=debut;i<=fin;i++) 
      s += p[i];

return s ;
}

double moyenne(double *p, double w, int debut, int fin){
double moy=0.0;
  for(int i=debut;i<=fin;i++) 
      moy += i*p[i];
  if (w !=0.0)
    moy /=w; 

return moy ;
}

double variance(double *p, double mu, double w, int debut, int fin){
double var=0.0;

  for(int i=debut;i<=fin;i++) 
      var += (i-mu)*(i-mu)*p[i];
  if(w !=0.0)
   var /=w;
return var ;
}

int main (int argc, char *argv[])
{
int k, k_opt;
double min, w1, w2, mu1, mu2, sigma1, sigma2, sigmaw;
double *p;

 IplImage* img=cvLoadImage(argv[1], 0);
    if(!img){
        std::cout << "could not load image file: " << argv[1] << std::endl ;
        exit(0);
    }    

  IplImage* dst_f=cvCreateImage(cvGetSize(img),IPL_DEPTH_8U,1);

  min = 10000000000;
  p = histo(img);
  for(k=0;k<256;k++){
     w1 = somme(p,0,k);
     w2 = somme(p,k+1,255);

     mu1 = moyenne(p,w1,0,k);
     mu2 = moyenne(p,w1,k+1,255);

     sigma1 = variance(p,mu1,w1,0,k);
     sigma2 = variance(p,mu2,w2,k+1,255);

     sigmaw = w1*sigma1+w2*sigma2;

        if(sigmaw<min){
         min = sigmaw;
	     k_opt = k;
	    }
    }

  cvThreshold(img,dst_f,k_opt,255,CV_THRESH_BINARY);
  cout << k_opt << endl;

    cvNamedWindow("mainWin", CV_WINDOW_AUTOSIZE);
    cvMoveWindow("mainWin", 100, 100);
    cvShowImage("mainWin",img);
    cvNamedWindow("Sobel", CV_WINDOW_AUTOSIZE);
    cvShowImage("Sobel",dst_f);
    cvWaitKey(0);
    cvReleaseImage(&img);
    cvReleaseImage(&dst_f);

}