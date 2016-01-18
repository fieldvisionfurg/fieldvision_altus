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

/*
Constructor method.
*/
LineDetectorHT::LineDetectorHT() 
{

}

/*
Destructor method.
*/
LineDetectorHT::~LineDetectorHT()
{
}

std::vector<Line<>> LineDetectorHT::detect(const cv::Mat& img) const
{
		cv::Mat outImg,outColor;
		cv::Canny( img, outImg, 120, 130, 3 );
		
		std::vector<cv::Vec4i> linesCv;
        std::vector<Line<>> lines;

        cv::HoughLinesP( outImg, linesCv, 1, CV_PI/180, 80, 30, 10 );
		for( size_t i = 0; i < linesCv.size(); i++ )
		{
			cv::Point p0,p1;
			p0.x=linesCv[i][0];
			p0.y=linesCv[i][1];
			p1.x=linesCv[i][2];
			p1.x=linesCv[i][3];

			Line<> li;
			li.p1=p0;
			li.p2=p1;

			lines.push_back(li);
		}
		return lines;

}

}
