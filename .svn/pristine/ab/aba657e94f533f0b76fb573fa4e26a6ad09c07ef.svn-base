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

//Main include
#include "VisionImplementationCv.h"

#ifndef _CONTOURSMOOTH_H
#define _CONTOURSMOOTH_H

#pragma once
class ContourSmooth
{
public:

	//splits a point vector in x and y vectors
	void PolyLineSplit(const std::vector<cv::Point2i>& pl,std::vector<int>& contourx, std::vector<int>& contoury) {
		contourx.resize(pl.size()); 
		contoury.resize(pl.size());

		for (unsigned j=0; j<pl.size(); j++) 
		{ 
			contourx[j] = (int)(pl[j].x); 
			contoury[j] = (int)(pl[j].y); 
		}
	}

	//joins two vectors (x and y) into points
	void PolyLineMerge(std::vector<cv::Point2i>& pl, const std::vector<int>& contourx, const std::vector<int>& contoury) {
		assert(contourx.size()==contoury.size());
		pl.resize(contourx.size());
		for (unsigned j=0; j<contourx.size(); j++) {
			pl[j].x = (int)(contourx[j]);
			pl[j].y = (int)(contoury[j]);
		}
	}

	// smooth a vector of points
	std::vector<int> smoothVector(const std::vector<int>& in, int neighbors = 10);

	// smooth a vector of vector of points
	// similar to the results of a vectorized image in corel or inkscape
	std::vector<std::vector<cv::Point2i> > smoothContours(const std::vector<std::vector<cv::Point2i> > &contours, int neighbors = 10);

	// smooth a vector of vector of points
	// the neighbors are calculated in relation to each contour
	// bigger contours are most affected than small ones
	std::vector<std::vector<cv::Point2i>> smoothContoursP(const std::vector<std::vector<cv::Point2i>> &contours, float neighbors = .1);

	//Constructor
	ContourSmooth(void);
	~ContourSmooth(void);
};

#endif 
