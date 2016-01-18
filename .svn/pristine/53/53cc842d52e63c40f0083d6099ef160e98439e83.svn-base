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
Default constructor method.
*/
MultiObjectTrackerCCCv::MultiObjectTrackerCCCv()
    : AbstractAutoTracker<cv::Mat,cv::Rect>()
{
	m_lostTrack=true; 
}

/*
Constructor method.
*/
MultiObjectTrackerCCCv::MultiObjectTrackerCCCv(const std::string& cascadeFile)
    : AbstractAutoTracker<cv::Mat,cv::Rect>(new ObjectDetectorCCCv(cascadeFile))
    , m_cascadePtr()

{
    m_cascadePtr=((ObjectDetectorCCCv*)m_detector)->getCascadePtr();
}


/*
Destructor method.
*/
MultiObjectTrackerCCCv::~MultiObjectTrackerCCCv()
{
}


VisionCore::Tracker<cv::Mat,cv::Rect>* MultiObjectTrackerCCCv::createTracker(const cv::Rect& initialObj)
{
    return new ObjectTrackerCCCv(m_cascadePtr,initialObj);
}


VisionCore::Detector<cv::Mat,cv::Rect>& MultiObjectTrackerCCCv::getDetector()
{
    return *m_detector;
}

void MultiObjectTrackerCCCv::setCascade(const std::string& cascadeFile)
{
    ((ObjectDetectorCCCv*)m_detector)->setCascade(cascadeFile);
	m_lostTrack=false; // assume que se tem um detector, tá rastreando
}


bool MultiObjectTrackerCCCv::isSameObject(const cv::Rect& obj1,
                                          const cv::Rect& obj2)
{
	bool sameObject;
    //TODO: improve this algorithm
    const double areaObj1 = obj1.area();
    cv::Rect objIntersection=obj1 & obj2;
	const double areaIntersect=objIntersection.area();
	const double ratio= areaIntersect/areaObj1;
	if(ratio>0.7)
        sameObject=true;
    else
        sameObject=false;
            
    return sameObject;
}

}
