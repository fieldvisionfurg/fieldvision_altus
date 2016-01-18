/*
Copyright (c) 2015, FURG - Universidade Federal do Rio Grande
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
    * Neither the name of the Universidade Federal do Rio Grande nor the
      names of its contributors may be used to endorse or promote products
      derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL UNIVERSIDADE FEDERAL DO RIO GRANDE BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

// Master include file
#include "VisionImplementationCv.h"

namespace Viscv {

/*
Constructor method.
*/
CircleDetectorHTCF::CircleDetectorHTCF() 
	: useColorFilter(false)
	, useBlur(true)
	, showProcessingImgs(false)
	, meanHue(120)
	, varHue(10)
	, gaussFilterSigma(1/300.0)
	, houghParam1(100)
	, houghParam2(200)
{

}


/*
Destructor method.
*/
CircleDetectorHTCF::~CircleDetectorHTCF()
{
}


/*
Retorna uma imagem contendo somente os pixels com valores entre os limites especificados por meanHue e varHue.
*/
void CircleDetectorHTCF::colorFilter(const cv::Mat& src, cv::Mat& dst) const
{
	using namespace cv;
	// http://docs.opencv.org/trunk/doc/py_tutorials/py_imgproc/py_colorspaces/py_colorspaces.html

	// Converte para HSV
	Mat hsv;
	cvtColor(src,hsv,CV_BGR2HSV);

	// Filtra
	Scalar minValue(std::max(0,meanHue-varHue),50,50);
	Scalar maxValue(std::min(179,meanHue+varHue),255,255);
	inRange(hsv,minValue,maxValue,dst);
}

std::vector<Circle<>> CircleDetectorHTCF::detect(const cv::Mat& img) const
{
    using namespace cv;

	// IMAGE PRE-PROCESSING
	Mat gray,blured,dst;
	const double sigma = img.rows * gaussFilterSigma;
	if(useBlur && useColorFilter){
		//apply blur first, then color filter
		GaussianBlur( img, blured, Size(0,0), sigma, sigma );
		colorFilter(blured,dst);
	}
	if(!useBlur && useColorFilter){
		colorFilter(img,dst);
	}
	if(useBlur && !useColorFilter){
		if(img.channels()>1)
			cvtColor(img, gray, CV_BGR2GRAY);
		else
			gray=img;
		GaussianBlur( gray, dst, Size(0,0), sigma, sigma );
	}
	if(!useBlur && !useColorFilter){
		if(img.channels()>1)
			cvtColor(img, dst, CV_BGR2GRAY);
		else
			dst=img;
	}

	if(showProcessingImgs){ imshow("Pre-processed image",dst); cvWaitKey(1);}

	// APPLY HOUGH TRANSFORM
    vector<Vec3f> circles;
    HoughCircles(dst, circles, CV_HOUGH_GRADIENT,
		2, dst.rows/8, houghParam1, houghParam2 );
    
    std::vector<Circle<>> lc;
    for(const Vec3f& c : circles)
        lc.push_back(Circle<>((int)c[0],(int)c[1],(int)c[2]));
    return lc;
}
}
