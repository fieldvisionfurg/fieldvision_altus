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

#include "ContourSmooth.h"



//smooth for one vector
std::vector<int> ContourSmooth::smoothVector(const std::vector<int>& in, int neighbors){
	cv::Mat out = cv::Mat(in);	
	if(neighbors){//when the neighbors value is 0
		cv::blur(out, out, cv::Size(1, neighbors));
	}
	return out;
}


//Given a vector of vectors it returns a mean smoothed result
std::vector<std::vector<cv::Point2i> > ContourSmooth::smoothContours(const std::vector<std::vector<cv::Point2i> >& contours, int neighbors){
	
	std::vector<std::vector<cv::Point> > smoothedBorders;
	
	for(unsigned i = 0; i < contours.size(); i++){
		std::vector<cv::Point> smooth;
		std::vector<int> contourx, contoury;
		//Splits poins into vectors	
		PolyLineSplit(contours[i], contourx, contoury);
		
		//blurring of X and Y
		contourx = smoothVector(contourx, neighbors);
		contoury = smoothVector(contoury, neighbors);
		
		//Merges smoothed vector
		PolyLineMerge(smooth, contourx, contoury);

		smoothedBorders.push_back(smooth);
	}

	return smoothedBorders;
}

// smooth a vector of vector of points
// the neighbors are calculated in relation to each contour
// bigger contours are most affected than small ones
std::vector<std::vector<cv::Point2i> > ContourSmooth::smoothContoursP(const std::vector<std::vector<cv::Point2i> > &contours, float neighbors){
	std::vector<std::vector<cv::Point> > smoothedBorders;
	
	for(unsigned i = 0; i < contours.size(); i++){
		std::vector<cv::Point> smooth;
		std::vector<int> contourx, contoury;
		//Splits poins into vectors	
		PolyLineSplit(contours[i], contourx, contoury);
		
		//blurring of X and Y
        contourx = smoothVector(contourx, static_cast<int>( contourx.size() * neighbors));
        contoury = smoothVector(contoury, static_cast<int>( contourx.size() * neighbors));
		
		//Merges smoothed vector
		PolyLineMerge(smooth, contourx, contoury);

		smoothedBorders.push_back(smooth);
	}

	return smoothedBorders;
}

//Constructor
ContourSmooth::ContourSmooth(void)
{
}

//Destructor
ContourSmooth::~ContourSmooth(void)
{
}
