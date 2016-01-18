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



#ifndef _FIRECOLORDETECTORCT_H
#define _FIRECOLORDETECTORCT_H

#pragma once
class FireColorDetectorCT
{
public:

    static cv::Mat getThresholdedImageSquared(cv::Mat imgBGR);

	//This function threshold the HSV image and create a binary image
	static cv::Mat getThresholdedImageHSV(cv::Mat &imgBGR, cv::Scalar color1 = cv::Scalar(0,0,0), cv::Scalar color2 = cv::Scalar(75,256,256));

	// This is a color filter based on a method proposed in "Fire Detection Based on Video Processing Method"
	// by Ti Nguyen-Ti,Thuan Nguyen-Phuc, Tuan Do-Hong (2013)
	// first rule:
	//	(R > G > B) & (R-B > 90) & (G-B > 40)
	// second rule (Determining max value and min value of fire in R, G, B)
	//	 (R > 190) & (G > 100) & (B < 180)
	// 3th rule (Determining fire pixel in YCbCr)
	// (Y > Cb) & (Cr > Cb)
	// 4th rule (Threshold fire value in YCbCr space)
	// ( Cb - Cr >= 30)
	// 5th rule (Determining max, min values of fire in Cb, Cr planes)
	//  ( Cb <= 120) & (Cr > 150)
	// 6th rule (yellow)
	// 0 se (R > 190) & (R-G <= 25) & (G-B >= 65)
	// 7th rule (Brilliant yellow)
	// 0 se (190 < R < 225) & (35 <= R-G <= 80) & (G-B >=70)
	// 8ht rule (Skin colour)
	// 0 se (190 < R < 230) & (35 <= R-G <= 80) & (G-B < 30)
	// 9th rule (It is applied to add pixels at centred flame where the flame is the brightest)
	// (R > 250) & (R-G < 5) & (G-B >= 70)
	// rules are applied
	// R1&R2&R3&R4&R5&R6&R7&R8|R9
	static cv::Mat getThresholdedImageNguyenTi(cv::Mat imgBGR, int minRValue = 190, int minGValue = 100, int maxBValue = 180, int minRBDiff = 10, int minGBDiff = 10,
		int minCbCrDiff = 30, int maxCbValue = 120, int minCrValue = 150);

	//This is a simple threshold filter with experimental thresholds:
	//	r > rt (red threshold)
	//	g > gt (green threshold)
	//	b < bt (blue threshold)
	static cv::Mat getThresholdedImageBGR(cv::Mat &imgBGR, cv::Scalar color1 = cv::Scalar(0,0,0), cv::Scalar color2 = cv::Scalar(100,100,256));

	//This filter is based on Early Fire Detection Based on Flame Contours in Video - 
	// Xiao-Lin Zhou, Fa-Xin Yu, Yu-Chun Wen, Zhe- Ming Lu and Guang-Hua Song (2010)
	// Paper can be find here http://scialert.net/fulltext/?doi=itj.2010.899.908&org=11
	// This is a simple threshold filter with experimental thresholds:
	//	r > rt (red threshold)
	//	g > gt (green threshold)
	//	b < bt (blue threshold)
	// RT, GT and BT are the thresholds of R, G and B components which are defined according to various experimental results,
	// ranging from 120 to 180, 70 to 110 and 30 to 50 respectively.
	static cv::Mat getThresholdedImageBGRZhou(cv::Mat &imgBGR, cv::Scalar color1 = cv::Scalar(30,70,120), cv::Scalar color2 = cv::Scalar(255,255,255), int minFirePixels = 20, int dilateKernelSize = 2);

	//This is a simple threshold filter with experimental thresholds:
	//	r > rt (red threshold)
	//	g > gt (green threshold)
	//	b < bt (blue threshold)
    static cv::Mat getThresholdedImageBGR2(cv::Mat &imgBGR);

	//This is a color filter based on a method proposed in "Fire Detection Using Statistical
	//Color Model in Video Sequences" by Turgay Celik, Hasan Demeril, Huseyin Ozkaramanli, Mustafa Uyguroglu
	// Article download http://www.sciencedirect.com/science/article/pii/S1047320306000927 
	//This method uses the RGB color space and does three comparisons.e
	//The method returns true at any pixel that satisfies:
	//red > green > blue
	//red > red threshold (depends on amount of light in image)
	static cv::Mat getThresholdedImageBGRCelik(cv::Mat &imgBGR, int minRedValue = 220);

	//This is a color filter based on a method proposed in "Fire Detection Using Statistical
	//Color Model in Video Sequences" by Turgay Celik, Hasan Demeril, Huseyin Ozkaramanli, Mustafa Uyguroglu - 2007
	// Article download http://www.sciencedirect.com/science/article/pii/S1047320306000927
	//This method uses the RGB color space and does three comparisons.e
	//The method returns true at any pixel that satisfies:
	//red > green > blue
	//red > red mean (depends on amount of light in image)
	// green / (red+1) <=0.65
	// blue / (red +1) <=0.45
	// blue / (green+1) <=0.6 
	static cv::Mat getThresholdedImageBGRCelik2(cv::Mat &imgBGR); 

	//This is a color filter based on a method proposed in "An Early Fire-Detection Method Based on Image Processing" 
	//by Thou-Ho (Chao-Ho) Chen, Ping-Hsueh Wu, and Yung-Chuen Chiou - 2004
	//This method uses the RGB and HSI color spaces and does three comparisons.e
	//The method returns true at any pixel that satisfies:
	//rule I: R>RT 
	//rule2:R  2 G>B 
	//rule 3: S 2 ((255-R)*SdRr) 
	//IF (ruleI)AND(rule2)AND(ruIe3)=TRlJE 
	//THEN fire-pixel 
	//ELSE  notfire-pixel 
	//and  typical  values range from 55 to 65 and 115 to 135 for St and Rt, respectively
	static cv::Mat getThresholdedImageChen(cv::Mat &imgBGR, int redThreshold=135, int saturationThreshold=65);

	//L for lightness and a and b for the color-opponent dimensions
	//"Fast and Efficient Method for Fire Detection	Using Image Processing" by Turgay Celik - 2010
	//The method returns true at any pixel that satisfies:
	//L* > Lm* (mean of L* values)
	//a* > am* (mean of a* values)
	//b* > bm* (mean of b* values)
	//b* > a*
	static cv::Mat getThresholdedImageLABCelik(cv::Mat &imgBGR);



	//The method returns false at any pixel that satisfies:
	// Y <= Cb
	// Cr < Cb
	// Y < Ym
	// Cb >= Cbm
	// Cr <= Crm
	// (Cr - Cb) <55
	static cv::Mat getThresholdedImageYCbCr(cv::Mat &imgRGB);

	// Generate a gray level image according Zhou
	// Red value has more importance
	// gives a better representation for fire in most cases
	//((0.11 * ptr_b[x]) + (0.3 * ptr_g[x]) + (0.59 * ptr_r[x]))
    static cv::Mat toGrayZhou(cv::Mat &imgBGR);

	// Generate a gray level image according Steffens - 2014
	// Real Time Flicker based Fire Flame Detection11for RGB cameras, submited to GCPR 2014
	// Reduces significantly the finded contours by merging pixels with similar values
	// Red value has more importance
	// (0.12P_B + 0.31P_G + 0.57P_R)/8 if  > 16;
	// 0 else
    static cv::Mat toGraySteffens(cv::Mat &imgBGR);

	

	//	GaussianR <= 168
	//|   GaussianR <= 149: S (37571.0/9.0)
	//|   GaussianR > 149
	//|   |   GaussianB <= 187: N (249.0)
	//|   |   GaussianB > 187: S (2778.0/1.0)
	//GaussianR > 168
	//|   GaussianB <= 234: N (32342.0/11.0)
	//|   GaussianB > 234: S (768.0/26.0)
	//Steffens - 2014 based on data mining of over 73k instances and trained using a C4.5 algorithm in Weka
    static cv::Mat getGaussianRGBFiltered(cv::Mat &imgBGR);

	//Default constructor and destructor
	FireColorDetectorCT(void);
	~FireColorDetectorCT(void);
};
#endif // ndef _FIRECOLORDETECTOR_H

