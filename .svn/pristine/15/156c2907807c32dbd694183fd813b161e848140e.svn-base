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

#include "VisionImplementationCv.h"


#ifndef _MOTIONDETECTOR_H
#define _MOTIONDETECTOR_H

#pragma once
class MotionDetector
{
private:
	cv::Mat prevFrame, currentFrame, nextFrame;
	cv::Mat lastBinaryImage, currentBinaryFrame;
	std::vector<cv::Mat> lastBinaryFrames;

public:
	// Finds the movement based on the last 3 frames
	// frame is the current frame
	// binaryMotion is a bynary image with the motion regions
	void collinsMotion(cv::Mat frame, cv::Mat &binaryMotion, int minDifference = 20);

	// sets a new frame for Contour based motion
	// you can't use collins and contours together
	// please chose one of them and have fun
	// you can input both gray scale or BGR
	void contoursMotion(cv::Mat frame, cv::Mat &borders, int minDifference = 0);

	// sets a new frame for Contour based motion
	// you can't use collins and contours together
	// please chose one of them and have fun
	// you can input both gray scale or BGR
	void frameDiff(cv::Mat frame, cv::Mat &borders, int minDifference = 0);

	//Returns the the amount of changes in the last n frames
	void getChangeCount(cv::Mat &changes);

	//Returns the Absolute Difference amount of changes in the last n frames
	void getChangeAbsDiff(cv::Mat &changes);

	//Default Constructor and destructor
	MotionDetector(void);
	~MotionDetector(void);

	//public variables
	unsigned historySize;
};


#endif // ndef _MOTIONDETECTOR_H
