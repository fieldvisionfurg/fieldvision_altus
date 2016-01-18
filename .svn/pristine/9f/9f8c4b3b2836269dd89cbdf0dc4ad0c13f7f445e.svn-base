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


ColorBlobDetectorHF::ColorBlobDetectorHF(void)
	: minHsv(100,50,50)
	, maxHsv(140,255,255)
	, erodeSize(7)
	, dilateSize(7)
	, showProcessingImgs(false)
{
}


ColorBlobDetectorHF::~ColorBlobDetectorHF(void)
{
}

std::vector<std::vector<cv::Point>> ColorBlobDetectorHF::detect(const cv::Mat& img) const
{
	cv::Mat hsvImg,filteredImg,dilateImg,erodeImg;

	// Converte imagem para HSV
	cvtColor(img,hsvImg,CV_BGR2HSV);

	// Filtra (saida é binária)
	inRange(hsvImg,minHsv,maxHsv,filteredImg);
	if(showProcessingImgs) {imshow("Filtered",filteredImg);}

	// Dilatação
	cv::Mat kernelDil = cv::getStructuringElement(cv::MORPH_ELLIPSE,cv::Size(dilateSize,dilateSize));
	cv::dilate(filteredImg,dilateImg,kernelDil);
	if(showProcessingImgs) {imshow("First dilation",dilateImg);}

	// Erosão (aplica duas vezes para eliminar pequenos blobs)
	cv::Mat kernelEro = cv::getStructuringElement(cv::MORPH_ELLIPSE,cv::Size(erodeSize,erodeSize));
	cv::erode(dilateImg,erodeImg,kernelEro,cv::Point(-1,-1),2);
	if(showProcessingImgs) {imshow("Erosion",erodeImg);}

	// Dilatação
	cv::dilate(erodeImg,dilateImg,kernelDil);
	if(showProcessingImgs) {imshow("Second dilation",dilateImg); cvWaitKey(1);}

	// Acha contornos
	std::vector<std::vector<cv::Point>> contours;
	cv::Mat hierarchy;
	cv::findContours(dilateImg,contours,hierarchy,CV_RETR_EXTERNAL,CV_CHAIN_APPROX_SIMPLE);

	// Filtra contornos por tamanho ou forma
	// TODO

    return contours;
}

void ColorBlobDetectorHF::setMinHsv(const cv::Scalar& minHsv_)
{
	minHsv=minHsv_;
}

void ColorBlobDetectorHF::setMaxHsv(const cv::Scalar& maxHsv_)
{
	maxHsv=maxHsv_;
}

const cv::Scalar& ColorBlobDetectorHF::getMinHsv( ) const
{
	return minHsv;
}

const cv::Scalar& ColorBlobDetectorHF::getMaxHsv() const
{
	return maxHsv;
}
}
