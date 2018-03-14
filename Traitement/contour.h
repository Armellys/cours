#ifndef _CONTOUR_H_
#define _CONTOUR_H_


#include <stdio.h>
#include <highgui.h>
#include <opencv2/opencv.hpp>

/* ===========================
            CONSTANTES
   =========================== */

// Masque
const float kMoyenneur = 1.0;
//
const float kMaskDerivation[] = {-1.0, 1.0 }; 

// Operateur de Robert
const float kMaskRobert1[]    = {1.0,  0.0,
                                 0.0, -1.0 };

const float kMaskRobert2[]    = { 0.0, 1.0,
                                 -1.0, 0.0 };

// Operateur de Prewitt
const float kMaskPrewitt1[]   = { -1.0, 0.0, 1.0,
                                  -1.0, 0.0, 1.0,
                                  -1.0, 0.0, 1.0 };

const float kMaskPrewitt2[]   = { -1.0, -1.0, -1.0,
                                   0.0,  0.0,  0.0,
                                   1.0,  1.0,  1.0 };

// OPerateur de Sobel
const float kMaskSobel1[]     = { -1.0, 0.0, 1.0,
                                  -2.0, 0.0, 2.0,
                                  -1.0, 0.0, 1.0 };

const float kMaskSobel2[]     = { -1.0, -2.0, -1.0,
                                   0.0,  0.0,  0.0,
                                   1.0,  2.0,  1.0 };


/* ===========================
            PROTOTYPES
   =========================== */

// === Mask
void init_mask_float(cv::Mat& mask, const float values[]);

// === Convolution
cv::Mat convolution_float(const cv::Mat& src, const cv::Mat& mask);
float   calc_conv_grey_float(const cv::Mat& src, const cv::Mat& mask, int x, int y);

// === Misc
void    normalize_float(cv::Mat& src);
cv::Mat square_float(const cv::Mat& src);
cv::Mat module_gradient_float(const cv::Mat& h1, const cv::Mat& h2);

// === Print
void affiche_image(const cv::Mat& src, const cv::String name);

// === Operateurs
cv::Mat end_operateur(const cv::Mat& src, const cv::Mat& mask1, const cv::Mat& mask2);
cv::Mat robert(const cv::Mat& src);
cv::Mat prewitt(const cv::Mat& src);
cv::Mat sobel(const cv::Mat& src);

#endif