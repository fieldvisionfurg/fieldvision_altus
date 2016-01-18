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

#include "TemplMatchingTracker.h"


// Master include file
#include "VisionImplementationCv.h"

namespace Viscv
{

TemplMatchingTracker::TemplMatchingTracker()
    : enlargeRatio(1.3)
    , initialized(false)
{

}

TemplMatchingTracker::~TemplMatchingTracker()
{

}

cv::Mat TemplMatchingTracker::getTemplate() const
{
    return detector.getTemplate();
}

void TemplMatchingTracker::setTemplate(const cv::Mat &templ)
{
    detector.setTemplate(templ);
}
int TemplMatchingTracker::getMatchMethod() const
{
    return detector.getMatchMethod();
}

void TemplMatchingTracker::setMatchMethod(int value)
{
    detector.setMatchMethod(value);
}
double TemplMatchingTracker::getEnlargeRatio() const
{
    return enlargeRatio;
}

void TemplMatchingTracker::setEnlargeRatio(double value)
{
    enlargeRatio = value;
}


const cv::Rect& TemplMatchingTracker::getLastTrack()
{
    return track;
}

void TemplMatchingTracker::update(const VisionCore::Frame<cv::Mat> &frame)
{
    if(detector.ready()){
        const cv::Mat &img=frame.getImg();

        //creates a rectangle with the bounds of img
        cv::Rect imgRect(0,0,img.cols-1,img.rows-1);
        cv::Rect ROI;

        if(initialized){
            //define ROI around current object
            ROI.width=static_cast<int>(track.width*enlargeRatio);
            ROI.height=static_cast<int>(track.height*enlargeRatio);
            ROI.x=track.x+(track.width-ROI.width)/2;
            ROI.y=track.y+(track.height-ROI.height)/2;
            ROI&=imgRect; //intersection
        }
        else{
            //search all image
            ROI=imgRect;
        }

        std::vector<cv::Rect> det = detector.detect(img(ROI));

        if(det.size()==0){
            m_lostTrack=true;
        }
        else{
            track = det.at(0);
            track+=cv::Point(ROI.x,ROI.y); //this changes to input image coordinates
            initialized=true;
            m_lostTrack=false;
        }
    }
    else{
        m_lostTrack=true;
    }
}

void TemplMatchingTracker::reset(const cv::Rect& rectangle)
{
    track=rectangle;
    m_lostTrack=false;
    initialized=true;
}


} //namespace Viscv
