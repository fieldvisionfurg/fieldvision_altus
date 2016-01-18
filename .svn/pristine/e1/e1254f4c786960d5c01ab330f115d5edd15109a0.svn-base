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

#include <opencv/cv.h>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <vector>

#pragma once
class FrameAnnotation
{

	//Baseado nestes artigos getters e setters s�o uma droga em C++
	//http://peripateticaxiom.blogspot.com.br/2008/06/tdd-mocks-and-design.html
	//http://typicalprogrammer.com/doing-it-wrong-getters-and-setters/
public:
	int frameNumber;   
	std::vector<cv::Rect> rectOnFrame;

	void addRect(cv::Rect r);
	FrameAnnotation(int frameNum, std::vector<cv::Rect> rects);

	/**
	 * Merge a set of rectangles if there's an overlap between each rectangle for more than specified overlap area
	 * @param   overlap the minimum area of overlap before 2 rectangles are merged
	 * @param   group_threshold only the rectangles that have more than the remaining group_threshold rectangles will be retained
	 **/
    void mergeRectangles(float overlap, unsigned group_threshold );
	
	//Default Constructor and destructor
	FrameAnnotation(void);
	~FrameAnnotation(void);
};
