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
Default constructor.
*/
ObjectDetectorCCCv::ObjectDetectorCCCv()
	:m_scaleFactor(1.3)
	,m_minNeighbors(2)
	,m_minSizeProp(0.1)
{
}

/*
Constructor method.
*/
ObjectDetectorCCCv::ObjectDetectorCCCv(const std::string& cascadeFile) 
    : m_cascadePtr(std::shared_ptr<cv::CascadeClassifier>(new cv::CascadeClassifier(cascadeFile)))
	,m_scaleFactor(1.3)
	,m_minNeighbors(2)
	,m_minSizeProp(0.1)
{
    if(m_cascadePtr->empty())
        printf("Face detector did not load properly\n");

}


/*
Constructor method.
*/
ObjectDetectorCCCv::ObjectDetectorCCCv(std::shared_ptr<cv::CascadeClassifier>& cascadePtr)
    : m_cascadePtr(cascadePtr)
	,m_scaleFactor(1.3)
	,m_minNeighbors(2)
	,m_minSizeProp(0.1)
{
}

void ObjectDetectorCCCv::detect(const cv::Mat &img, std::vector<cv::Rect> &obj) const {
    for(cv::Rect d : this->detect(img))
        obj.push_back(d);
}


/*
Destructor method.
*/
ObjectDetectorCCCv::~ObjectDetectorCCCv()
{
}


std::vector<cv::Rect> ObjectDetectorCCCv::detect(const cv::Mat& img) const
{
    std::vector<cv::Rect> objects;
    assert(img.rows > 0 && img.cols > 0);
    m_cascadePtr->detectMultiScale(img,objects,m_scaleFactor,m_minNeighbors,0 | CV_HAAR_SCALE_IMAGE,cv::Size((int)(m_minSizeProp*img.rows),(int)(m_minSizeProp*img.rows)));
    return objects;
}

void ObjectDetectorCCCv::setCascade(const std::string& cascadeFile)
{
	m_cascadePtr = std::shared_ptr<cv::CascadeClassifier>(new cv::CascadeClassifier(cascadeFile));
	if(m_cascadePtr->empty())
        printf("Cascade file did not load properly\n");
}


void ObjectDetectorCCCv::setCascade(std::shared_ptr<cv::CascadeClassifier>& cascadePtr)
{
	m_cascadePtr = cascadePtr;
}


/*
Returns the value of member 'm_cascadePtr'.
*/
const std::shared_ptr<cv::CascadeClassifier>& ObjectDetectorCCCv::getCascadePtr() const
{
    return m_cascadePtr;
}

}
