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
ObjectTrackerCCCv::ObjectTrackerCCCv()
    : m_cascadePtr()
	, m_currentTrack(cv::Rect(0,0,10,10))
    , m_incROI(1.5)
    , m_redROI(0.7)
{
	m_lostTrack=true;
}

/*
Constructor method.
*/
ObjectTrackerCCCv::ObjectTrackerCCCv(std::shared_ptr<cv::CascadeClassifier> cascadePtr,
                                     const cv::Rect& initialObj)
    : m_cascadePtr(cascadePtr)
    , m_currentTrack(initialObj)
    , m_incROI(1.5)
    , m_redROI(0.7)
{
}


/*
Constructor method.
*/
ObjectTrackerCCCv::ObjectTrackerCCCv(const std::string& cascadeFile,
                                     const cv::Rect& initialObj) 
    : m_cascadePtr(std::make_shared<cv::CascadeClassifier>(cascadeFile))
    , m_currentTrack(initialObj)
    , m_incROI(1.5)
    , m_redROI(0.7)
{
}


/*
Destructor method.
*/
ObjectTrackerCCCv::~ObjectTrackerCCCv()
{
}


const cv::Rect& ObjectTrackerCCCv::getLastTrack()
{
    return m_currentTrack;
}

void ObjectTrackerCCCv::reset(const cv::Rect& rectangle)
{
	m_currentTrack=rectangle;
	m_lostTrack=false;
}

cv::Rect& ObjectTrackerCCCv::resizeRect(cv::Rect& rectangle, const double scale)
{
    const int oldWidth=rectangle.width;
    const int oldHeight=rectangle.height;
    
    rectangle.width=static_cast<int>(rectangle.width*scale);
    rectangle.height=static_cast<int>(rectangle.height*scale);
    
    rectangle.x=rectangle.x+(oldWidth-rectangle.width)/2;
    rectangle.y=rectangle.y+(oldHeight-rectangle.height)/2;
    
    return rectangle;
}


/*
Given an image (i.e. video frame), update the tracked objects.
*/
void ObjectTrackerCCCv::update(const VisionCore::Frame<cv::Mat>& frame)
{

    const cv::Mat &img=frame.getImg(); 
	const double incROI = m_incROI; // amount to increase search
	const double redROI = m_redROI; // amount to reduce search
        
	//creates a rectangle with the bounds of img
	cv::Rect imgRect(0,0,img.cols-1,img.rows-1);
    
	//define ROI around current face
	cv::Rect ROI(m_currentTrack); 
	ObjectTrackerCCCv::resizeRect(ROI,incROI);
    ROI&=imgRect; //intersection
        
	//minimum size of face to search
	int minHeight = cvRound(m_currentTrack.height*redROI);
	int minWidth  = cvRound(m_currentTrack.width*redROI);
        
	//perform face detection
    std::vector<cv::Rect> faces;  
	faces.reserve(10);
	this->m_cascadePtr->detectMultiScale(img(ROI),faces,1.3,2,0,cv::Size(minHeight,minWidth));
    if(faces.size()>0){
		//TODO: what if more than one face is found???
        m_currentTrack=faces[0]; //this coordinates are wrt ROI
		m_currentTrack+=cv::Point(ROI.x,ROI.y); //this changes to input image coordinates
        m_lostTrack=false;
    }
    else{
        m_lostTrack=true;
    }
}

}
