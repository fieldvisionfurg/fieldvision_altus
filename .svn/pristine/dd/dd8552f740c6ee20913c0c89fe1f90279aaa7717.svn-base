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

#include <time.h>

namespace Viscv {

/*
Constructor method.
*/
FrameServerCv::FrameServerCv(const std::string& filename)
    : m_capture(filename)
	, m_frameCount(0)
{
	if(!m_capture.isOpened())
		printf("FrameServerCv::FrameServerCv - Video didn't load...\n");
   
}


/*
Constructor method.
*/
FrameServerCv::FrameServerCv(const int device) 
    : m_capture(device)
	, m_frameCount(0)
{
	//m_frameHeight=m_capture.get(CV_CAP_PROP_FRAME_HEIGHT);
	//m_frameWidth=m_capture.get(CV_CAP_PROP_FRAME_WIDTH);
	//m_frameFormat=m_capture.get(CV_CAP_PROP_FORMAT);
	if(!m_capture.isOpened())
		printf("FrameServerCv::FrameServerCv - Camera didn't load...\n");
    //m_capture.set(CV_CAP_PROP_FRAME_WIDTH,1280);
    //m_capture.set(CV_CAP_PROP_FRAME_HEIGHT,960);
    
}

FrameServerCv::FrameServerCv(const int device, const int width, const int height)
    : m_capture(device)
    , m_frameCount(0)
{
    if(!m_capture.isOpened())
        printf("FrameServerCv::FrameServerCv - Camera didn't load...\n");
    m_capture.set(CV_CAP_PROP_FRAME_WIDTH,width);
    m_capture.set(CV_CAP_PROP_FRAME_HEIGHT,height);
}


/*
Destructor method.
*/
FrameServerCv::~FrameServerCv()
{
    releaseServer();
}


/*
Captures the next frame available.
*/
const VisionCore::Frame<cv::Mat> FrameServerCv::captureFrame()
{
	cv::Mat *img = new cv::Mat;
	m_capture.grab();
	m_capture.retrieve(*img);

	
    //cv::cvtColor( *img, *img, CV_BGR2GRAY );
	m_frameCount++;
	double timestamp =m_capture.get(CV_CAP_PROP_POS_MSEC);
	if(timestamp<=0.0)
		timestamp=time(NULL)*1000.0;
    return VisionCore::Frame<cv::Mat>(img,timestamp,m_frameCount);
}


/*
Return true if there exist more frames available.
*/
bool FrameServerCv::hasNext()
{
	if(!m_capture.isOpened())
		return false;
	else{
		double a=m_capture.get(CV_CAP_PROP_POS_FRAMES); //current frame
		double b=m_capture.get(CV_CAP_PROP_FRAME_COUNT); //total frames
        //POS_FRAME starts at 0, therefore it must be a < b
        return (static_cast<int>(a) < static_cast<int>(b)) || (static_cast<int>(b)==-1);
	}
}


void FrameServerCv::releaseServer()
{
    m_capture.release();
}

}
