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

namespace Viscv{


/*
Constructor method.
*/
CircleTrackerHTCF::CircleTrackerHTCF(int x, int y,
                                     int radius)
    : m_lastTrack(x, y, radius)
    , m_lostTrack(false)
{

}


/*
Destructor method.
*/
CircleTrackerHTCF::~CircleTrackerHTCF()
{
}


const Circle<>& CircleTrackerHTCF::getLastTrack()
{
	return m_lastTrack;
}


/*
Given an image (i.e. video frame), update the tracked objects.
*/
void CircleTrackerHTCF::update(const VisionCore::Frame<cv::Mat>& frame)
{
    using namespace cv;

    const Mat& img = frame.getImg();
    Mat gray;
    // smooth it, otherwise a lot of false circles may be detected
	const double sigma = img.rows * 1/300.0;

    GaussianBlur( img, gray, Size(0,0), sigma, sigma );
    vector<Vec3f> circles;
    
    // help: http://docs.opencv.org/modules/imgproc/doc/feature_detection.html
    HoughCircles(gray, circles, CV_HOUGH_GRADIENT,
                 2, gray.rows, 200, 200,m_lastTrack.m_radius/2,m_lastTrack.m_radius*2 );
    
    if(circles.size()==0)
        m_lostTrack=true;
    else{
        const Vec3f& c = *circles.begin();
        m_lastTrack.m_x=static_cast<int>(c[0]);
        m_lastTrack.m_y=static_cast<int>(c[1]);
        m_lastTrack.m_radius=static_cast<int>(c[2]);
    }
}
}
