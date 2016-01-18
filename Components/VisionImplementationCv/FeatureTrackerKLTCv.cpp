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
FeatureTrackerKLTCv::FeatureTrackerKLTCv(std::vector<cv::Point2f> points) //@INIT_1709
    : m_previousImg()
    , m_points(points)
    , m_previousPoints()
{
}


/*
Constructor method.
*/
FeatureTrackerKLTCv::FeatureTrackerKLTCv(const cv::Mat& img,
                                         const cv::Rect& roi) //@INIT_1716
    : m_previousImg()
    , m_points()
    , m_previousPoints()
{
    addPointsFromRegion(img,roi);
}


/*
Constructor method.
*/
FeatureTrackerKLTCv::FeatureTrackerKLTCv() 
    : m_previousImg()
    , m_points()
    , m_previousPoints()
{
}


/*
Destructor method.
*/
FeatureTrackerKLTCv::~FeatureTrackerKLTCv()
{
}


void FeatureTrackerKLTCv::addPointToTrack(const cv::Point2f& point)
{
    m_points.push_back(point);
}

void FeatureTrackerKLTCv::addPointsFromRegion(const cv::Mat &img,const cv::Rect& roi)
{
    cv::Mat imgGray;
    cv::cvtColor(img, imgGray, CV_BGR2GRAY);

    double qualityLevel = 0.05;
    double minDistance = 5.0;
    int maxCorners = 20;
    std::vector<cv::Point2f> points;
    goodFeaturesToTrack(imgGray(roi),points,maxCorners,qualityLevel,minDistance);
    //the points are wrt roi axis. Convert to img axis.
    for(cv::Point2f& p : points){
        p.x+=roi.x;
        p.y+=roi.y;
        this->addPointToTrack(p);
    }


}

void FeatureTrackerKLTCv::reset()
{
    m_points.clear();
    m_previousImg = cv::Mat();
    m_previousPoints.clear();
}


void FeatureTrackerKLTCv::cvMouseCallback(int clickEvent, int x, int y,
                                          int, void* featureTrackerPtr)
{
    FeatureTrackerKLTCv* ofPtr = (FeatureTrackerKLTCv*)featureTrackerPtr;
    if(ofPtr != NULL && clickEvent == CV_EVENT_LBUTTONDOWN){
        ofPtr->addPointToTrack(cv::Point2f((float)x, (float)y));
    }
}


const std::vector<cv::Point2f>& FeatureTrackerKLTCv::getLastTrack()
{
    return m_points;
}


/*
Given an image (i.e. video frame), update the tracked objects.
*/
void FeatureTrackerKLTCv::update(const VisionCore::Frame<cv::Mat>& frame)
{
	//the method only works for gray images, so convert if necessary
	cv::Mat img;
	if(frame.getImg().channels()>1)
		cv::cvtColor(frame.getImg(), img, CV_BGR2GRAY);
	else
		img=frame.getImg();

	//if there is no previous img, get the current
    if(m_previousImg.empty()){
        if(m_previousImg.channels()>1)
			cv::cvtColor(frame.getImg(), m_previousImg, CV_BGR2GRAY);
		else
			m_previousImg=frame.getImg();
    }

	//computes optical flow
    else{
        if(m_points.size()>0){
			std::vector<cv::Point2f> result;
            int maxCorners=50;
            std::vector<uchar> features_found; 
            features_found.reserve(maxCorners);
	        std::vector<float> feature_errors; 
	        feature_errors.reserve(maxCorners);
			m_previousPoints=m_points;
            cv::calcOpticalFlowPyrLK( m_previousImg,img, m_previousPoints, m_points, features_found, feature_errors);
        }
		m_previousImg=img;
    }
}
}
