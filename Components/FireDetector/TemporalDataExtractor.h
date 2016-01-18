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


#ifndef _TemporalDataExtractor_H
#define _TemporalDataExtractor_H

#pragma once
class TemporalDataExtractor
{
private:
	int f_x, f_y, f_c;
	
	//channel, pixel, values
	// mat is transformed into a std::vector for processing
	std::vector<std::vector<cv::Mat>> chn_hist;

	//
		
	void initialize(int x,int y,int c);
public:
	// Include a new frame
	void addNewFrame(cv::Mat frame);
	//Compute the pixel mean
	void computePixelMean();
	void computePixelStdDev();

	// Converts a mat to float std::vector in the format channel > pixel_values.
	// Ex: RGB Mat is converted into a [3][cols*rows] std::vector
	std::vector<std::vector<float>> mat2vector(cv::Mat &m);
	
	// Converts the std::vector to mat.
	// The method supports only single channel Mat.
	cv::Mat vector2Mat(std::vector<float> &v, int x, int y);

	//Default Constructor and destructor
	TemporalDataExtractor(void);
	~TemporalDataExtractor(void);

	//public variables
	unsigned historySize;
	std::vector<cv::Mat> ch_mean, ch_std_dev, ch_variance;

	//show debug info
	bool show_dbg_info;
	
};


#endif // ndef _TemporalDataExtractor_H
