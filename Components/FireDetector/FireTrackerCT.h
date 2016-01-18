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
#include "FireColorDetectorCT.h"
#include "MotionDetector.h"


#ifndef _FIRETRACKERCT_H
#define _FIRETRACKERCT_H

#define FIRE_COLOUR_HSV_THRESHOLD 1
#define FIRE_COLOUR_NGUYEN_2013 2
#define FIRE_COLOUR_ZHOU_2010 3
#define FIRE_COLOUR_CELIK_2009 4
#define FIRE_COLOUR_CELIK_2007 5
#define FIRE_COLOUR_CELIK_2010 6
#define FIRE_COLOUR_YCbCr 7
#define FIRE_COLOUR_BRG_THRESHOLD 8
#define FIRE_COLOUR_CHAO_2004 9
#define FIRE_COLOUR_BRG_GAUSSIAN 10

#define GRAY_STEFFENS 1
#define GRAY_MEAN 2
#define GRAY_ZHOU 3
#define GRAY_ONLY_RED_CHANNEL 4

#define MOTION_COLLINS 1
#define MOTION_CONTOUR_BASED 2

#pragma once
class FireTrackerCT
    : public VisionCore::Tracker<cv::Mat, std::vector<std::vector<cv::Point>>>
{
private:
	//movement detector
    MotionDetector mvmt;

    std::vector<std::vector<cv::Point>> m_currentTrack;
    bool m_lostTrack;

	int inCheckedContour(cv::Mat &binaryFrame, std::vector<std::vector<cv::Point>> &testedContours, int index);
	void detectFireRegion(cv::Mat frame, int dilatation ,  double generalThreshold );
	void regionFilling(const cv::Mat &in, cv::Mat &out, int thresh , int maxThresh );
    void findFirePosition(cv::Mat thres, cv::Mat &grayScale, std::list<cv::Rect> &fireRect);

public:
	//constructor
	FireTrackerCT(int historySize = 10, int minMovement = 8, int motionMethod = MOTION_COLLINS, 
		int colourMethod = FIRE_COLOUR_HSV_THRESHOLD, int grayMethod = GRAY_STEFFENS);
	~FireTrackerCT(void);

	///Colour segmentation Method
	int _colourMethod;
	int _motionMethod;
	int _minMovement;
	int _historySize;
	int _grayMethod;

    const std::vector<std::vector<cv::Point>>& getLastTrack();
    bool lostTrack() const;
    /// Given an image (i.e. video frame), update the tracked objects.
    void update(const VisionCore::Frame<cv::Mat>& frame);

};


#endif // ndef _FIRETRACKERCT_H
