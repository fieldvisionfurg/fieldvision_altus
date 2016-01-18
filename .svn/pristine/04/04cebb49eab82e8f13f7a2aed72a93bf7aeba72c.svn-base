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

#include "FireTrackerCT.h"

//Constructor
/*
//Params:
// historySize is how many frames are evaluated to check motion
// minMovement is the minimal times the pixel value has to change in order to be considered as fire like movement
// motionMethod is the flickering method
// colourMethod is the method used to make the colour segmentation
*/
FireTrackerCT::FireTrackerCT(int historySize, int minMovement, int motionMethod, int colourMethod, int grayMethod)
{
	_historySize = historySize; _minMovement = minMovement; _motionMethod = motionMethod; _colourMethod = colourMethod; _grayMethod = grayMethod;
	mvmt.historySize = _historySize;
}

FireTrackerCT::~FireTrackerCT(void)
{
}

inline bool FireTrackerCT::lostTrack() const
{
	return false;
	//return m_lostTrack;
}

const std::vector<std::vector<cv::Point>>& FireTrackerCT::getLastTrack()
{
	
    return m_currentTrack;
}

///
void FireTrackerCT::update(const VisionCore::Frame<cv::Mat>& frame) {
	cv::Mat img = frame.getImg();
	cv::Mat bin, changeCount, grayFrame, colorFiltered;

		
	//Filters the colours in order to choose only fire coloured regions
	switch (_colourMethod){



	case FIRE_COLOUR_HSV_THRESHOLD:
		colorFiltered = FireColorDetectorCT::getThresholdedImageHSV(img, cv::Scalar(0,0,200), cv::Scalar(75,150,256));
		break;
	case FIRE_COLOUR_NGUYEN_2013:
		colorFiltered = FireColorDetectorCT::getThresholdedImageNguyenTi(img);
		break;
	case FIRE_COLOUR_ZHOU_2010:
		colorFiltered = FireColorDetectorCT::getThresholdedImageBGRZhou(img);
		break;
	case FIRE_COLOUR_CHAO_2004:
		colorFiltered = FireColorDetectorCT::getThresholdedImageChen(img);
		break;
	case FIRE_COLOUR_CELIK_2009:
		colorFiltered = FireColorDetectorCT::getThresholdedImageBGRCelik(img);
		break;
	case FIRE_COLOUR_CELIK_2007:
		colorFiltered = FireColorDetectorCT::getThresholdedImageBGRCelik2(img);
		break;
	case FIRE_COLOUR_CELIK_2010:
		colorFiltered = FireColorDetectorCT::getThresholdedImageLABCelik(img);
		break;
	case FIRE_COLOUR_YCbCr:
		colorFiltered = FireColorDetectorCT::getThresholdedImageYCbCr(img);
		break;
	case FIRE_COLOUR_BRG_THRESHOLD:
		colorFiltered = FireColorDetectorCT::getThresholdedImageBGR(img);
		break;
	case FIRE_COLOUR_BRG_GAUSSIAN:
		colorFiltered = FireColorDetectorCT::getGaussianRGBFiltered(img);
		//cv::imshow("colorFiltered", colorFiltered);
		break;
	default:
		colorFiltered = FireColorDetectorCT::getThresholdedImageHSV(img, cv::Scalar(0,0,200), cv::Scalar(75,150,256));
		//TODO...
	}

	//Gray scale method
	std::vector<cv::Mat> bgr_channels;
	switch (_grayMethod){
	case GRAY_STEFFENS:
		grayFrame = FireColorDetectorCT::toGraySteffens(img);
		break;
	case GRAY_ZHOU:
		grayFrame = FireColorDetectorCT::toGrayZhou(img);
		break;
	case GRAY_MEAN:
		cv::cvtColor(img, grayFrame, cv::COLOR_BGR2GRAY);
		break;
	case GRAY_ONLY_RED_CHANNEL:
		cv::split(img, bgr_channels);
		grayFrame = bgr_channels[2].clone();
		break;
	default:
		grayFrame = FireColorDetectorCT::toGraySteffens(img);
		break;
	}

	//Motion
	switch (_motionMethod){
	case MOTION_COLLINS:
		mvmt.collinsMotion(grayFrame, bin, 20);
		mvmt.getChangeCount(changeCount);
		//apply the min movement threshold
		cv::threshold(changeCount, changeCount, _minMovement, _historySize, cv::THRESH_BINARY);
		break;
	case MOTION_CONTOUR_BASED:
		mvmt.contoursMotion(grayFrame, bin, _minMovement);
		//mvmt.contoursMotion(grayFrame, bin, 0);
		mvmt.getChangeAbsDiff(changeCount);
		break;
	default:
		mvmt.collinsMotion(img, bin, 30);
		break;
		//TODO...
	}
	


	
	/// Find contours
	std::vector<std::vector<cv::Point> > contoursColor;
	std::vector<cv::Vec4i> hierarchy;
	cv::Mat tmp;
	//bitwise and between the gray image and color segmentation
	cv::bitwise_and(grayFrame, colorFiltered, tmp);

	cv::findContours( tmp, contoursColor, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, cv::Point(0, 0));
	for(unsigned i=0; i< contoursColor.size(); i++){
		approxPolyDP( cv::Mat(contoursColor[i]), contoursColor[i], 1, true );
	}

	cv::Mat k = cv::getGaussianKernel(5, .7);
	//bitwise and between the gray image and color segmentation
	dilate(changeCount, changeCount, k);
	cv::bitwise_and(colorFiltered, changeCount, changeCount);
	//erode(changeCount, changeCount, cv::Mat(2,2,changeCount.type()));
	
	
	m_currentTrack.clear();
	//parallel for
#pragma loop(hint_parallel(0))
	for( unsigned i = 0; i< contoursColor.size(); i++ )
	{
		// contour
		if(20 < contourArea(contoursColor[i])){
			if(inCheckedContour(changeCount, contoursColor, i)){
				m_currentTrack.push_back(contoursColor[i]);
				
			}
		}
	}

#if 0
	for( int i = 0; i< contoursColor.size(); i++ )
	{
		drawContours( img, contoursColor, i, cv::Scalar(0,255,0), 2, 8, vector<cv::Vec4i>(), 0, cv::Point() );
	}
	for( int i = 0; i< m_currentTrack.size(); i++ )
	{
		drawContours( img, m_currentTrack, i, cv::Scalar(255,0,0), 2, 8, vector<cv::Vec4i>(), 0, cv::Point() );
	}
	cv::normalize(changeCount, changeCount, 0, 255, cv::NORM_MINMAX);
	cv::imshow("changes", changeCount);
	cv::normalize(grayFrame, grayFrame, 0, 255, cv::NORM_MINMAX);
	cv::imshow("grayFrame", grayFrame);
	cv::imshow("colorFiltered", colorFiltered);
	cv::imshow("frame", img);
#endif

}

int FireTrackerCT::inCheckedContour(cv::Mat &binaryFrame, std::vector<std::vector<cv::Point> > &testedContours, int index){
	// Suppose this has the contours of just the car and the obstacle.
	// create an image filled with zeros, single-channel, same size as img.
	cv::Mat tested = cv::Mat::zeros(binaryFrame.size(), CV_8UC1 );

	// contour
	drawContours( tested, testedContours, index, cv::Scalar::all(255), -1, 8, std::vector<cv::Vec4i>(), 0, cv::Point() );

	// now AND the two together
	bitwise_and( binaryFrame, tested, tested);
	cv::Scalar soma = sum(tested);
    return static_cast<int>(soma[0]);
}


/**
* Fills the region between to points by finding his external contours and drawing filled polygons
*/
void FireTrackerCT::regionFilling(const cv::Mat &in, cv::Mat &out, int thresh , int maxThresh ){
	cv::Mat img_gray,img_bw,canny_output,src;

	cv::normalize(in, src, 0, 255, cv::NORM_MINMAX, CV_8UC1);
	out = src.clone();
	out.setTo(cv::Scalar(0));
	std::vector<std::vector<cv::Point> > contours;
	std::vector<cv::Vec4i> hierarchy;

	cv::Canny( src, canny_output, thresh, maxThresh, 3 );
	cv::dilate(canny_output, canny_output, cv::Mat(), cv::Point(-1,-1));

	cv::findContours( src, contours, hierarchy, cv::RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, cv::Point(0, 0) );

	std::vector<cv::Point> approxShape;
	for(size_t i = 0; i < contours.size(); i++){
		approxPolyDP(contours[i], approxShape, cv::arcLength(cv::Mat(contours[i]), true)*0.04, true);
		drawContours(out, contours, i, cv::Scalar(255), CV_FILLED);   // fill
	}

}


/**
*Finds the biggest fire regions
*/
void FireTrackerCT::findFirePosition(cv::Mat thres, cv::Mat &grayScale, std::list<cv::Rect> &fireRect ){
	
	cv::morphologyEx(thres, thres, cv::MORPH_OPEN, cv::Mat());

	cv::Mat threshold_output;
	std::vector<std::vector<cv::Point> > contours;
	std::vector<cv::Vec4i> hierarchy;

	/// Find contours
	cv::findContours( thres, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, cv::Point(0, 0) );
	int highestMean = 0;


	#pragma loop(hint_parallel(0))
	for( unsigned i = 0; i< contours.size(); i++ ) // iterate through each contour. 
	{
		// Get the mean bright
		// Get ROI image.
		cv::Rect roi(cv::boundingRect(contours[i]));
		cv::Mat crop(grayScale, roi);

		// Calculate ROI mean.
		cv::Mat mask(cv::Mat::zeros(crop.rows, crop.cols, CV_8UC1)); //the mask with the size of cropped image
		cv::drawContours(mask, contours, i, cv::Scalar(255), CV_FILLED);
		auto mean(cv::mean(crop, mask));
		//


		if(mean[0] > highestMean){
			//Store the index of largest contour
			// Find the bounding rectangle for biggest contour
            highestMean = static_cast<int>(mean[0]);
		}
	}

	#pragma loop(hint_parallel(0))
	for( unsigned i = 0; i< contours.size(); i++ )  
	{
		// iterate through each contour
		// Get the mean bright
		// Get ROI image.
		cv::Rect roi(cv::boundingRect(contours[i]));
		cv::Mat crop(grayScale, roi);

		// Calculate ROI mean.
		cv::Mat mask(cv::Mat::zeros(crop.rows, crop.cols, CV_8UC1)); //the mask with the size of cropped image
		// The offset for drawContours has to be *minus* roi.tl();
		cv::drawContours(mask, contours, i, cv::Scalar(255), CV_FILLED);
		auto mean(cv::mean(crop, mask));

		if (mean[0] > highestMean * 0)
		{
			//Only the biggest area rectangles are returned
			fireRect.push_back(boundingRect(contours[i]));
		}
	}
}
