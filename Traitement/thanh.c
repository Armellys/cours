/* ===========================
             MEMO
   ===========================
Le type float prend des valeurs entre 0 et 1 !

*/

/* ===========================
            INCLUDES
   =========================== */

#include "contour.h"

/* ===========================
           NAMESPACES
   =========================== */

using namespace cv;
using namespace std;


/* ===========================
              MAIN 
   =========================== */

int main(int argc, char const *argv[])
{
    if(argc < 3)
    {
        cout << "Usage: ./app fp_image contour" << endl
        //cout << "Usage: ./app fp_image contour colored" << endl
        << "fp_image: Filepath to source image" << endl
        << "contour:  0|1|2 (robert|prewitt|sobel)" << endl;
        //<< "colored:  0 for grayscale, >0 for colored" << endl;
        return 0;
    }
    //const int kColored  = strtol(argv[2], NULL, 10);

    Mat image = imread(argv[1], CV_LOAD_IMAGE_GRAYSCALE);
    if(!image.data)
    {
        printf("No image data \n");
        return -1;
    }

    Mat result;
    switch(strtol(argv[2], NULL, 10))
    {
        case 0: result = robert(image);  break;
        case 1: result = prewitt(image); break;
        case 2: result = sobel(image);   break;
        default: 
            cout << "contour:  0|1|2 (robert|prewitt|sobel)" << endl;
            return 1;
    }

    affiche_image(result, "result");
    affiche_image(image, "image");

    waitKey(0);
    return 0;
}


/* ===========================
            FONCTIONS
   =========================== */

// =========================== MASK
void init_mask_float(Mat& mask, const float values[])
{
    int nb_rows = mask.size().height;

    for(int y=0; y<mask.size().width; y++)
        for(int x=0; x<mask.size().height; x++)
            mask.at<float>(x,y) = values[x*nb_rows + y];

    cout << mask << endl;
}


// =========================== CONVOLUTION
Mat convolution_float(const Mat& src, const Mat& mask)
{
    // La convolution reduit la taille de 1 en x et y
    int dimX = src.size().height-1;  
    int dimY = src.size().width-1;

    Mat result = Mat(dimX, dimY, CV_32FC1);
    for(int y=1; y<dimY; y++)
        for(int x=1; x<dimX; x++)
            result.at<float>(x,y) = calc_conv_grey_float(src, mask, x, y);

    normalize_float(result);
    return result;
}

float calc_conv_grey_float(const Mat& src, const Mat& mask, int x, int y)
{   
    int dimMaskX = mask.size().height;
    int dimMaskY = mask.size().width;
    int offsetX  = dimMaskX/2;
    int offsetY  = dimMaskY/2;

    // Effet de bord si dimensions pairs
    int limitY   = (dimMaskY%2==0 ? offsetY : offsetY+1);
    int limitX   = (dimMaskX%2==0 ? offsetX : offsetX+1);

    // Algo de convolution generalise pour toute matrice
    float res(0.0);
    for(int j=(-offsetY); j<limitY; j++)
        for(int i=(-offsetX); i<limitX; i++)
            res += (float)src.at<uchar>(x-i, y-j) * mask.at<float>(i+offsetX,j+offsetY);

    return res/kMoyenneur;
}

// =========================== MISC
void normalize_float(Mat& src)
{
    float max(0.00000000001);

    // Recherche du max
    for(int y=0; y<src.size().width; y++)
        for(int x=0; x<src.size().height; x++)
            if(src.at<float>(x,y)>max)
                max = src.at<float>(x,y);


    // Normalisation [0, 1]
    for(int y=0; y<src.size().width; y++)
        for(int x=0; x<src.size().height; x++)
            src.at<float>(x,y) /= max;
}

Mat square_float(const Mat& src)
{
    Mat result = Mat(src.size().height, src.size().width, CV_32FC1);

    for(int y=1; y<src.size().width; y++)
        for(int x=1; x<src.size().height; x++)
            result.at<float>(x,y) = src.at<float>(x, y)*src.at<float>(x, y);

    return result;
}

Mat module_gradient_float(const Mat& h1, const Mat& h2)
{    
    Mat result = Mat(h1.size().height, h1.size().width, CV_32FC1);
    result     = square_float(h1) + square_float(h2);

    for(int y=0; y<h1.size().width; y++)
        for(int x=0; x<h1.size().height; x++)
            result.at<float>(x,y) = sqrt(result.at<float>(x,y));

    normalize_float(result);
    return result;
}

// =========================== PRINT
void affiche_image(const Mat& src, String name)
{
    namedWindow(name, WINDOW_AUTOSIZE );
    imshow(name, src);
}

// =========================== OPERATEURS
Mat end_operateur(const Mat& src, const Mat& mask1, const Mat& mask2)
{
    Mat h1 = convolution_float(src, mask1);
    Mat h2 = convolution_float(src, mask2);

    affiche_image(h1, "h1");
    affiche_image(h2, "h2");

    return module_gradient_float(h1, h2);
}

Mat robert(const Mat& src)
{
    Mat mask1 = Mat(2, 2, CV_32FC1);
    Mat mask2 = Mat(2, 2, CV_32FC1);

    init_mask_float(mask1, kMaskRobert1);
    init_mask_float(mask2, kMaskRobert2);

    return end_operateur(src, mask1, mask2);
}

Mat prewitt(const Mat& src)
{
    Mat mask1 = Mat(3, 3, CV_32FC1);
    Mat mask2 = Mat(3, 3, CV_32FC1);

    init_mask_float(mask1, kMaskPrewitt1);
    init_mask_float(mask2, kMaskPrewitt2);

    return end_operateur(src, mask1, mask2);
}

Mat sobel(const Mat& src)
{
    Mat mask1 = Mat(3, 3, CV_32FC1);
    Mat mask2 = Mat(3, 3, CV_32FC1);

    init_mask_float(mask1, kMaskSobel1);
    init_mask_float(mask2, kMaskSobel2);

    return end_operateur(src, mask1, mask2);
}