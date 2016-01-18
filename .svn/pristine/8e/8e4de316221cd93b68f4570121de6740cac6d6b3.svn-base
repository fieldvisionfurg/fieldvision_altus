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

#include "FireColorDetectorCT.h"

cv::Mat FireColorDetectorCT::getThresholdedImageSquared(cv::Mat imgBGR){
	//This is a simple threshold filter with experimental thresholds:
	//	r > rt (red threshold)
	//	g > gt (green threshold)
	//	b < bt (blue threshold)
	cv::Mat imgThresh;
	imgThresh.create(imgBGR.rows, imgBGR.cols, CV_8UC1);

	std::vector<cv::Mat> planes;
	//Copy each color to his own array
	imgBGR = imgBGR + imgBGR;
	cv::normalize(imgBGR, imgBGR, 0, 255, cv::NORM_MINMAX, CV_8UC1);
	cv::split(imgBGR.clone(), planes); 

	//gets the mean value of each channel
	cv::Scalar meanValues = cv::mean(imgBGR);
    int bAvg = static_cast<int>(meanValues[0]);
    int gAvg = static_cast<int>(meanValues[1]);
    int rAvg = static_cast<int>(meanValues[2]);

	//for each row in the image
	for( int y = 0; y < imgBGR.rows; y++ )
	{
		uchar* ptr_b = planes[0].ptr<uchar>(y);
		uchar* ptr_g = planes[1].ptr<uchar>(y);
		uchar* ptr_r = planes[2].ptr<uchar>(y);

		uchar *ptr_bin = imgThresh.ptr<uchar>(y);
		//for each column in the image
		for( int x = 0; x < imgBGR.cols; x++ )
		{
			ptr_bin[x] = 255;
			if (ptr_b[x] <= bAvg)
				ptr_bin[x] = 0;
			if (ptr_g[x] <= gAvg)
				ptr_bin[x] = 0;
			if (ptr_r[x] <= rAvg)
				ptr_bin[x] = 0;
			if (ptr_r[x] <= ptr_b[x])
				ptr_bin[x] = 0;
			if (ptr_r[x] <= ptr_g[x])
				ptr_bin[x] = 0;

		}
	}
	merge(planes, imgBGR);
	cv::normalize(imgBGR, imgBGR, 0, 255, cv::NORM_MINMAX, CV_8UC1);
	cv::Mat result;
	cv::cvtColor(imgBGR, result, CV_BGR2GRAY);
	GaussianBlur(result, result, cv::Size(5,5), 1);
	return result;
}

cv::Mat FireColorDetectorCT::getThresholdedImageHSV(cv::Mat &imgBGR, cv::Scalar color1, cv::Scalar color2){
	//This function threshold the HSV image and create a binary image
	cv::Mat imgHSV;
	cv::cvtColor(imgBGR, imgHSV, CV_BGR2HSV); //Change the color forcv::Mat from BGR to HSV
	cv::inRange(imgHSV, color1, color2, imgHSV); 
	//cv::normalize(imgHSV, imgHSV,0, 255, cv::NORM_MINMAX);
	return imgHSV;
}

cv::Mat FireColorDetectorCT::getThresholdedImageNguyenTi(cv::Mat imgBGR, int minRValue, int minGValue, int maxBValue, int minRBDiff, int minGBDiff,
														 int minCbCrDiff, int maxCbValue, int minCrValue){ 
															 //cv::Mat getThresholdedImageNguyenTi(cv::Mat imgBGR, int minRValue = 190, int minGValue = 100, int maxBValue = 180, int minRBDiff = 90, int minGBDiff = 40,
															 //int minCbCrDiff = 30, int maxCbValue = 120, int minCrValue = 150){ 
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

															 //r... é cada uma das regras
															 cv::Mat r1, r2, r3, r4, r5, r6, r7, r8, r9, imgThresh, imgYCrCb;

															 // cria a imagem em YCrCb
															 cv::cvtColor(imgBGR, imgYCrCb, CV_BGR2YCrCb);

															 //inicializa cv::Matriz de resultados de cada etapa
															 imgThresh.create(imgBGR.rows, imgBGR.cols, CV_8UC1);
															 r1.create(imgBGR.rows, imgBGR.cols, CV_8UC1);
															 r2.create(imgBGR.rows, imgBGR.cols, CV_8UC1);
															 r3.create(imgBGR.rows, imgBGR.cols, CV_8UC1);
															 r4.create(imgBGR.rows, imgBGR.cols, CV_8UC1);
															 r5.create(imgBGR.rows, imgBGR.cols, CV_8UC1);
															 r6.create(imgBGR.rows, imgBGR.cols, CV_8UC1);
															 r7.create(imgBGR.rows, imgBGR.cols, CV_8UC1);
															 r8.create(imgBGR.rows, imgBGR.cols, CV_8UC1);
															 r9.create(imgBGR.rows, imgBGR.cols, CV_8UC1);


															 //Copy each color to his own array
															 std::vector<cv::Mat> planesBGR, planesYCrCb;
															 cv::split(imgBGR, planesBGR); 
															 cv::split(imgYCrCb, planesYCrCb); 


															 //for each row in the image
															 for( int y = 0; y < imgBGR.rows; y++ )
															 {
																 uchar* ptr_b = planesBGR[0].ptr<uchar>(y);
																 uchar* ptr_g = planesBGR[1].ptr<uchar>(y);
																 uchar* ptr_r = planesBGR[2].ptr<uchar>(y);
																 uchar* ptr_y = planesYCrCb[0].ptr<uchar>(y);
																 uchar* ptr_Cr = planesYCrCb[1].ptr<uchar>(y);
																 uchar* ptr_Cb = planesYCrCb[2].ptr<uchar>(y);

																 //uchar *ptr_bin = imgThresh.ptr<uchar>(y);
																 uchar *ptr_r1 = r1.ptr<uchar>(y);
																 uchar *ptr_r2 = r2.ptr<uchar>(y);
																 uchar *ptr_r3 = r3.ptr<uchar>(y);
																 uchar *ptr_r4 = r4.ptr<uchar>(y);
																 uchar *ptr_r5 = r5.ptr<uchar>(y);
																 uchar *ptr_r6 = r6.ptr<uchar>(y);
																 uchar *ptr_r7 = r7.ptr<uchar>(y);
																 uchar *ptr_r8 = r8.ptr<uchar>(y);
																 uchar *ptr_r9 = r9.ptr<uchar>(y);
																 //for each column in the image

																 for( int x = 0; x < imgBGR.cols; x++ )
																 {
																	 int rgdiff = ptr_r[x] - ptr_g[x];
																	 int gbdiff = ptr_g[x] - ptr_b[x];
																	 int rbdiff = ptr_r[x] - ptr_b[x];
																	 // 1 rule
																	 ptr_r1[x] = 0;
																	 if (ptr_r[x] >= ptr_g[x] && ptr_g[x] >= ptr_b[x] && rbdiff >= minRBDiff &&  gbdiff >= minGBDiff)
																		 ptr_r1[x] = 1;
																	 // 2 rule
																	 ptr_r2[x] = 0; 
																	 if (ptr_r[x] >= minRValue && ptr_g[x] >= minGValue && ptr_b[x] < maxBValue)
																		 ptr_r2[x] = 1;
																	 // 3 rule
																	 ptr_r3[x] = 0; 
																	 if (ptr_y[x] > ptr_Cb[x] && ptr_Cr[x] > ptr_Cb[x])
																		 ptr_r3[x] = 1;
																	 // 4 rule
																	 ptr_r4[x] = 0; 
																	 if (ptr_Cb[x] - ptr_Cr[x] > minCbCrDiff)
																		 ptr_r4[x] = 1;
																	 // 5 rule
																	 ptr_r5[x] = 0; 
																	 if (ptr_Cb[x] < maxCbValue && ptr_Cr[x] > minCrValue)
																		 ptr_r5[x] = 1;
																	 // 6 rule
																	 ptr_r6[x] = 1; 
																	 if (ptr_r[x] > 190 && ptr_r[x] - ptr_g[x] <= 25 && ptr_g[x] - ptr_b[x] >= 65)
																		 ptr_r6[x] = 0;
																	 // 7 rule
																	 ptr_r7[x] = 1;
																	 if (190 < ptr_r[x] && ptr_r[x] < 225 &&  35 <= rgdiff && rgdiff <=60 && gbdiff >= 70)
																		 ptr_r7[x] = 0;
																	 // 8 rule skin
																	 ptr_r8[x] = 1;
																	 if (190 < ptr_r[x] && ptr_r[x] < 230 &&  35 <= rgdiff && rgdiff <=80 && gbdiff >= 30)
																		 ptr_r8[x] = 0;
																	 // 9 rule
																	 ptr_r9[x] = 0;
																	 if (ptr_r[x] > 250 &&  rgdiff <= 5 && gbdiff >= 70) //gbdiff do artigo é 70, mas não dá resultado algum com esses parâmetros
																		 ptr_r9[x] = 1;

																 }
															 }

															 bitwise_and(r1, r2, imgThresh);
															 bitwise_and(imgThresh, r3, imgThresh);
															 bitwise_and(imgThresh, r4, imgThresh);
															 bitwise_and(imgThresh, r5, imgThresh);
															 bitwise_and(imgThresh, r6, imgThresh);
															 bitwise_and(imgThresh, r7, imgThresh);
															 bitwise_and(imgThresh, r8, imgThresh);
															 bitwise_or(imgThresh, r9, imgThresh);
															 imgThresh = r4;
															 cv::normalize(imgThresh, imgThresh, 0, 255, cv::NORM_MINMAX, CV_8UC1);
															 return imgThresh;
}

cv::Mat FireColorDetectorCT::getThresholdedImageBGR(cv::Mat &imgBGR, cv::Scalar color1, cv::Scalar color2){
	//This is a simple threshold filter with experimental thresholds:
	//	r > rt (red threshold)
	//	g > gt (green threshold)
	//	b < bt (blue threshold)
	cv::Mat imgThresh;
	cv::inRange(imgBGR, color1, color2, imgThresh);
	bitwise_not(imgThresh, imgThresh);
	return imgThresh;
}

cv::Mat FireColorDetectorCT::getThresholdedImageBGRZhou(cv::Mat &imgBGR, cv::Scalar color1, cv::Scalar color2, int minFirePixels,int dilateKernelSize){
	//This filter is based on Early Fire Detection Based on Flame Contours in Video - 
	// Xiao-Lin Zhou, Fa-Xin Yu, Yu-Chun Wen, Zhe- Ming Lu and Guang-Hua Song (2010)
	// Paper can be find here http://scialert.net/fulltext/?doi=itj.2010.899.908&org=11
	// This is a simple threshold filter with experimental thresholds:
	//	r > rt (red threshold)
	//	g > gt (green threshold)
	//	b < bt (blue threshold)
	// RT, GT and BT are the thresholds of R, G and B components which are defined according to various experimental results,
	// ranging from 120 to 180, 70 to 110 and 30 to 50 respectively.
	cv::Mat imgThresh;
	cv::inRange(imgBGR, color1, color2, imgThresh);

	// Then we count the number of fire-colored pixels Sj in this frame. Finally, we determine whether the current frame is a fire-colored frame
	cv::Scalar fpCount = sum(imgThresh);
    int firePixelCount = static_cast<int>(fpCount[0]);
	if(firePixelCount > minFirePixels){
		//The flow chart of candidate fire frame selection process in the dashed block
		// In experiments, we find that there are small holes and isolated points in the binary image. In order to wipe off these noisy pixels or regions, we further smooth the binary image based on dilation and erosion operations.
		dilate(imgThresh, imgThresh, cv::Mat::ones(cv::Size(dilateKernelSize,dilateKernelSize),CV_8UC1));
		erode(imgThresh, imgThresh, cv::Mat::ones(cv::Size(dilateKernelSize,dilateKernelSize),CV_8UC1));

	}else{
		//if the fire pixels count is smaller than the minimum the frame is considered as no fire
		imgThresh = cv::Mat::zeros(imgThresh.size(), CV_8UC1);

	}

	return imgThresh;

} 

cv::Mat FireColorDetectorCT::getThresholdedImageBGR2(cv::Mat &imgBGR){
	//This is a simple threshold filter with experimental thresholds:
	//	r > rt (red threshold)
	//	g > gt (green threshold)
	//	b < bt (blue threshold)
	cv::Mat imgThresh;
	imgThresh.create(imgBGR.rows, imgBGR.cols, CV_8UC1);

	std::vector<cv::Mat> planes;
	//Copy each color to his own array
	cv::split(imgBGR, planes); 

	//gets the mean value of each channel
	cv::Scalar meanValues = mean(imgBGR);
    int bAvg = static_cast<int>(meanValues[0]);
    int gAvg = static_cast<int>(meanValues[1]);
    int rAvg = static_cast<int>(meanValues[2]);

	//for each row in the image
#pragma loop(hint_parallel(0))
	for( int y = 0; y < imgBGR.rows; y++ )
	{
		uchar* ptr_b = planes[0].ptr<uchar>(y);
		uchar* ptr_g = planes[1].ptr<uchar>(y);
		uchar* ptr_r = planes[2].ptr<uchar>(y);

		uchar *ptr_bin = imgThresh.ptr<uchar>(y);
		//for each column in the image
		for( int x = 0; x < imgBGR.cols; x++ )
		{
			ptr_bin[x] = 255;
			if (ptr_b[x] <= bAvg)
				ptr_bin[x] = 0;
			if (ptr_g[x] <= gAvg)
				ptr_bin[x] = 0;
			if (ptr_r[x] <= rAvg)
				ptr_bin[x] = 0;
			if (ptr_r[x] <= ptr_b[x])
				ptr_bin[x] = 0;
			if (ptr_r[x] <= ptr_g[x])
				ptr_bin[x] = 0;
		}
	}
	return imgThresh;
}

cv::Mat FireColorDetectorCT::getThresholdedImageBGRCelik(cv::Mat &imgBGR, int minRedValue){
	//This is a color filter based on a method proposed in "Fire detection in video sequences using a generic color model"
	//by Turgay Celik, Hasan Demirel [2009]
	//This method uses the RGB color space and does three comparisons.e
	//The method returns true at any pixel that satisfies:
	//red > green > blue
	//red > red threshold (depends on amount of light in image)
	cv::Mat imgThresh;
	imgThresh.create(imgBGR.rows, imgBGR.cols, CV_8UC1);

	cv::Mat planes[3];
	//Copy each color to his own array
	cv::split(imgBGR, planes); 

	//for each row in the image
#pragma loop(hint_parallel(0))
	for( int y = 0; y < imgBGR.rows; y++ )
	{
		uchar* ptr_b = planes[0].ptr<uchar>(y);
		uchar* ptr_g = planes[1].ptr<uchar>(y);
		uchar* ptr_r = planes[2].ptr<uchar>(y);

		uchar *ptr_bin = imgThresh.ptr<uchar>(y);
		//for each column in the image
		for( int x = 0; x < imgBGR.cols; x++ )
		{
			ptr_bin[x] = 0;
			if (ptr_r[x] >= ptr_g[x] && ptr_g[x] >= ptr_b[x] && ptr_r[x] >= minRedValue)
				ptr_bin[x] = 255;
		}
	}
	return imgThresh;
}

cv::Mat FireColorDetectorCT::getThresholdedImageBGRCelik2(cv::Mat &imgBGR){
	//This is a color filter based on a method proposed in "Fire Detection Using Statistical
	//Color Model in Video Sequences"[2007] by Turgay Celik, Hasan Demeril, Huseyin Ozkaramanli, Mustafa Uyguroglu
	// Article download http://www.sciencedirect.com/science/article/pii/S1047320306000927
	//This method uses the RGB color space and does three comparisons.e
	//The method returns true at any pixel that satisfies:
	//red > green > blue
	//red > red mean (depends on amount of light in image)
	// green / (red+1) <=0.65
	// blue / (red +1) <=0.45
	// blue / (green+1) <=0.6 
	cv::Mat imgThresh;
	imgThresh.create(imgBGR.rows, imgBGR.cols, CV_8UC1);

	cv::Mat planes[3];
	//Copy each color to his own array
	cv::split(imgBGR, planes); 
	cv::Scalar rm = mean(planes[2]);
    int redMean = static_cast<int>(rm[0]);
	//for each row in the image
#pragma loop(hint_parallel(0))
	for( int y = 0; y < imgBGR.rows; y++ )
	{
		uchar* ptr_b = planes[0].ptr<uchar>(y);
		uchar* ptr_g = planes[1].ptr<uchar>(y);
		uchar* ptr_r = planes[2].ptr<uchar>(y);
		uchar *ptr_bin = imgThresh.ptr<uchar>(y);
		//for each column in the image
		for( int x = 0; x < imgBGR.cols; x++ )
		{
			ptr_bin[x] = 0;
			if (ptr_r[x] >= redMean)
				if(ptr_r[x] > ptr_g[x] && ptr_g[x] > ptr_b[x])
					if(ptr_g[x]/(ptr_r[x]+1)<=0.65 && 
						ptr_b[x]/(ptr_r[x]+1)<=0.45 &&
						ptr_b[x]/(ptr_g[x]+1)<=0.6)
						ptr_bin[x] = 255;
		}
	}
	return imgThresh;
}

cv::Mat FireColorDetectorCT::getThresholdedImageChen(cv::Mat &imgBGR, int redThreshold, int saturationThreshold){
	//This is a color filter based on a method proposed in "An Early Fire-Detection Method Based on Image Processing" 
	//by Thou-Ho (Chao-Ho) Chen, Ping-Hsueh Wu, and Yung-Chuen Chiou - 2004
	//This method uses the RGB and HSI color spaces and does three comparisons.e
	//The method returns true at any pixel that satisfies:
	//rule I: R>RT 
	//rule2:R  2 G>B 
	//rule 3: S 2 ((255-R)*SdRr) 
	//IF (ruleI)AND(rule2)AND(ruIe3)=TRlJE 
	//THEN $re-pixel 
	//ELSE  notfire-pixel 
	cv::Mat imgThresh, imgHSV;

	cv::cvtColor(imgBGR, imgHSV, CV_BGR2HSV);

	imgThresh.create(imgBGR.rows, imgBGR.cols, CV_8UC1);
	imgThresh.setTo(0);

	std::vector<cv::Mat> planes, hsvPlanes;
	//Copy each color to his own array
	cv::split(imgBGR, planes); 
	cv::split(imgHSV, hsvPlanes);
	double sr = saturationThreshold/redThreshold;
	//for each row in the image
#pragma loop(hint_parallel(0))
	for( int y = 0; y < imgBGR.rows; y++ )
	{
		uchar* ptr_b = planes[0].ptr<uchar>(y);
		uchar* ptr_g = planes[1].ptr<uchar>(y);
		uchar* ptr_r = planes[2].ptr<uchar>(y);
		uchar* ptr_s = hsvPlanes[1].ptr<uchar>(y);

		uchar *ptr_bin = imgThresh.ptr<uchar>(y);
		//for each column in the image
#pragma loop(hint_parallel(0))
		for( int x = 0; x < imgBGR.cols; x++ )
		{
			if (ptr_r[x] >= redThreshold && ptr_r[x] >= ptr_g[x] && ptr_g[x] > ptr_b[x] && ptr_s[x] >= ((255 - ptr_r[x]) * sr) )
				ptr_bin[x] = 255;
		}
	}
	return imgThresh;
}


cv::Mat FireColorDetectorCT::getThresholdedImageLABCelik(cv::Mat &imgBGR){
	//L for lightness and a and b for the color-opponent dimensions
	//"Fast and Efficient Method for Fire Detection	Using Image Processing" by Turgay Celik - 2010
	//The method returns true at any pixel that satisfies all:
	//L* > Lm* (mean of L* values)
	//a* > am* (mean of a* values)
	//b* > bm* (mean of b* values)
	//b* > a*

	cv::Mat imgThresh, imgLAB;

	cv::cvtColor(imgBGR, imgLAB, CV_BGR2Lab); //Change the color forcv::Mat from BGR to Lab
	imgThresh.create(imgLAB.rows, imgLAB.cols, CV_8UC1);

	std::vector<cv::Mat> planes;
	//Copy each color to his own array
	cv::split(imgLAB, planes); 

	//gets the mean value of each channel
	cv::Scalar meanValues = cv::mean(imgLAB);
    int LAvg = static_cast<int>(meanValues[0]);
    int aAvg = static_cast<int>(meanValues[1]);
    int bAvg = static_cast<int>(meanValues[2]);

	//for each row in the image
	for( int y = 0; y < imgLAB.rows; y++ )
	{
		uchar* ptr_L = planes[0].ptr<uchar>(y);
		uchar* ptr_a = planes[1].ptr<uchar>(y);
		uchar* ptr_b = planes[2].ptr<uchar>(y);

		uchar *ptr_bin = imgThresh.ptr<uchar>(y);
		//for each column in the image
		for( int x = 0; x < imgLAB.cols; x++ )
		{
			ptr_bin[x] = 255;
			if (ptr_L[x] < LAvg || ptr_b[x] < bAvg || ptr_a[x] < aAvg || ptr_b[x] < ptr_a[x])
				ptr_bin[x] = 0;
		}
	}
	return imgThresh;

}


cv::Mat FireColorDetectorCT::getThresholdedImageYCbCr(cv::Mat &imgRGB){
	//Y - luminance, Cb - blue, Cr -red
	assert(imgRGB.channels() == 3);
	//creates the returned image
	cv::Mat imgThresh(imgRGB.rows, imgRGB.cols,CV_8UC1, 0);
	imgThresh.create(imgRGB.rows, imgRGB.cols, CV_8UC1);
	//works with YCbCr colorschema
	cv::Mat imgYCrCb;
	cv::cvtColor(imgRGB, imgYCrCb, CV_RGB2YCrCb);

	std::vector<cv::Mat> planes;
	//Copy each color to his own array
	cv::split(imgYCrCb, planes); 

	//gets the mean value of each channel
	cv::Scalar meanValues = cv::mean(imgYCrCb);
    int yAvg = static_cast<int>(meanValues[0]);
    int CrAvg = static_cast<int>(meanValues[1]);
    int CbAvg = static_cast<int>(meanValues[2]);

	//for each row in the image
#pragma loop(hint_parallel(0))
	for( int y = 0; y < imgYCrCb.rows; y++ )
	{
		uchar* ptr_y = planes[0].ptr<uchar>(y);
		uchar* ptr_Cr = planes[1].ptr<uchar>(y);
		uchar* ptr_Cb = planes[2].ptr<uchar>(y);

		uchar *ptr_bin = imgThresh.ptr<uchar>(y);
		//for each column in the image
		for( int x = 0; x < imgYCrCb.cols; x++ )
		{
			ptr_bin[x] = 255;
			if (ptr_y[x] <= ptr_Cb[x])
				ptr_bin[x] = 0;
			if (ptr_Cr[x] <= ptr_Cb[x])
				ptr_bin[x] = 0;
			if (ptr_y[x] <= yAvg)
				ptr_bin[x] = 0;
			if (ptr_Cb[x] >= CbAvg)
				ptr_bin[x] = 0;
			if (ptr_Cr[x] <= CrAvg)
				ptr_bin[x] = 0;
			if ((int)ptr_Cr[x] - (int)ptr_Cb[x] < 55)
				ptr_bin[x] = 0;
		}
	}
	return imgThresh;
}


cv::Mat FireColorDetectorCT::toGrayZhou(cv::Mat &imgBGR){
	//Flame region selection
	//After obtaining the candidate fire frames, Fj, j = 1, 2, …, 20, we turn to detect accurate flame regions in each candidate frame. For each frame Fj, we check each pixel p = (pR, pG, pB) in this frame and find the flame pixel by the following rule 

	// Copy each color to his own array
	// Generate a gray level image according Zhou
	// we convert it from the RGB color space into a gray-level image GLj by calculating the gray-level value for each flame pixel while setting each non-flame pixel to be 0 directly. For each pixel p = (pR, pG, pB) in this frame, we get the corresponding gray-level value pGL
	std::vector<cv::Mat> planesBGR;
	cv::Mat grayScale = cv::Mat::zeros(imgBGR.size(), CV_8UC1);

	cv::split(imgBGR, planesBGR);
	//for each row in the image
#pragma loop(hint_parallel(0))
	for( int y = 0; y < imgBGR.rows; y++ )
	{
		uchar* ptr_b = planesBGR[0].ptr<uchar>(y);
		uchar* ptr_g = planesBGR[1].ptr<uchar>(y);
		uchar* ptr_r = planesBGR[2].ptr<uchar>(y);
		uchar* ptr_gray = grayScale.ptr<uchar>(y);

		//for each column in the image
#pragma loop(hint_parallel(0))
		for( int x = 0; x < imgBGR.cols; x++ )
		{
            ptr_gray[x] = static_cast<uchar>((0.11 * ptr_b[x]) + (0.3 * ptr_g[x]) + (0.59 * ptr_r[x]));
		}
	}

	return grayScale;
}

cv::Mat FireColorDetectorCT::toGraySteffens(cv::Mat &imgBGR){
	//\begin{equation}
	//\label{eq:grayscaleEquation}
	//	P_{GL}(x,y) = 
	//		\begin{cases}
	//		\|(0.12P_B + 0.31P_G + 0.57P_R)/8\|, $ if $ > 16;\\
	//		0, $ else $\\
	//	\end{cases}
	//\end{equation} 
	// Copy each color to his own array
	// Generate a gray level image according 
	// we convert it from the RGB color space into a gray-level image GLj by calculating the gray-level value for each flame pixel while setting each non-flame pixel to be 0 directly. For each pixel p = (pR, pG, pB) in this frame, we get the corresponding gray-level value pGL
	std::vector<cv::Mat> planesBGR;
	cv::Mat grayScale = cv::Mat::zeros(imgBGR.size(), CV_8UC1);

	cv::split(imgBGR, planesBGR);
	//for each row in the image
#pragma loop(hint_parallel(0))
	for( int y = 0; y < imgBGR.rows; y++ )
	{
		uchar* ptr_b = planesBGR[0].ptr<uchar>(y);
		uchar* ptr_g = planesBGR[1].ptr<uchar>(y);
		uchar* ptr_r = planesBGR[2].ptr<uchar>(y);
		uchar* ptr_gray = grayScale.ptr<uchar>(y);

		//for each column in the image
#pragma loop(hint_parallel(0))
		for( int x = 0; x < imgBGR.cols; x++ )
		{
			double valueAtPixel = ((0.12 * ptr_b[x]) + (0.31 * ptr_g[x]) + (0.57 * ptr_r[x]))/8;
			if(valueAtPixel > 16){
                ptr_gray[x] = static_cast<uchar>(valueAtPixel);
			}else{
				ptr_gray[x] = 0;
			}
		}
	}

	return grayScale;
}


cv::Mat FireColorDetectorCT::getGaussianRGBFiltered(cv::Mat &imgBGR){
	//	GaussianB <= 168
	//|   GaussianB <= 149: S (37571.0/9.0)
	//|   GaussianB > 149
	//|   |   GaussianR <= 187: N (249.0)
	//|   |   GaussianR > 187: S (2778.0/1.0)
	//GaussianB > 168
	//|   GaussianR <= 234: N (32342.0/11.0)
	//|   GaussianR > 234: S (768.0/26.0)
	//Steffens - 2014 based on data mining of over 73k instances and trained using a C4.5 algorithm in Weka
	cv::Mat imgThresh;
	imgThresh.create(imgBGR.rows, imgBGR.cols, CV_8UC1);

	cv::Mat planes[3];
	//Copy each color to his own array
	cv::split(imgBGR, planes); 
	cv::GaussianBlur(planes[0], planes[0], cv::Size(9,9), .8);
	cv::GaussianBlur(planes[1], planes[1], cv::Size(9,9), .8);
	cv::GaussianBlur(planes[2], planes[2], cv::Size(9,9), .8);
	//for each row in the image
#pragma loop(hint_parallel(0))
	for( int y = 0; y < imgBGR.rows; y++ )
	{
		uchar* ptr_b = planes[0].ptr<uchar>(y);
//		uchar* ptr_g = planes[1].ptr<uchar>(y);
		uchar* ptr_r = planes[2].ptr<uchar>(y);

		uchar *ptr_bin = imgThresh.ptr<uchar>(y);
		//for each column in the image
		#pragma loop(hint_parallel(0))
		for( int x = 0; x < imgBGR.cols; x++ )
		{
			if (ptr_r[x] <= 168){
				if (ptr_r[x] <= 149) {ptr_bin[x] = 255;}
				else {
					if (ptr_b[x] < 187){ptr_bin[x] = 0;}
					else{ptr_bin[x] = 255;}
				}
			}
			else{
				if (ptr_b[x] <= 234){ptr_bin[x] = 0;}
				else {ptr_bin[x] = 255;}
			}
		}
	}
	cv::bitwise_not(imgThresh,imgThresh);
	return imgThresh;
}


//Default Constructor
FireColorDetectorCT::FireColorDetectorCT(void)
{
}


FireColorDetectorCT::~FireColorDetectorCT(void)
{
}
