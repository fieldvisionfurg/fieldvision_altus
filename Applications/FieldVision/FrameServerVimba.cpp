/*******************************************************************************\
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

/*******************************************************************************/
// Master include file
#include "VisionImplementationCv.h"
#include "FrameServerVimba.h"
#include <time.h>

using namespace Viscv;
using namespace VisionCore;

FrameServerVimba::FrameServerVimba(void)
{
	AVT::VmbAPI::CameraPtrVector cameras;
	AVT::VmbAPI::VimbaSystem &system = AVT::VmbAPI::VimbaSystem :: GetInstance ();

	// Open the first 1394 camera
	if ( VmbErrorSuccess == system.Startup () )
	{
		if ( VmbErrorSuccess == system.GetCameras( cameras ) )
		{
			for ( AVT::VmbAPI::CameraPtrVector :: iterator iter = cameras.begin ();
				cameras.end() != iter;
				++iter )
			{
				if ( VmbErrorSuccess != (*iter)->Open( VmbAccessModeFull ) )
				{
					std::cout << "Could not open the camera" << std::endl;
				}
			}
		}
	}

	if (cameras.size() == 0)
	{
        std::cout << "No camera detected" << std::endl;
        cameraOpened=false;
	}
    else{
        // Open the camera
        camera = cameras[0];
        cameraOpened=true;
    }

	//Read the features
	//AVT::VmbAPI::FeaturePtr feature;
	//VmbInt64_t width;

	//if ( VmbErrorSuccess == camera->GetFeatureByName( "Width", feature ) )
	//{
	//	if ( VmbErrorSuccess == feature ->GetValue( width ) )
	//	{
	//		std::cout << width << std::endl;
	//	}
	//}

}


FrameServerVimba::~FrameServerVimba(void)
{
	releaseServer();
}

/*
Captures the next frame available.
*/
const Frame<cv::Mat> FrameServerVimba::captureFrame()
{
	cv::Mat *image = new cv::Mat;
	m_frameCount++;
	double timestamp =time(NULL)*1000.0;

	//Read frame
	
	VmbErrorType err;
	AVT::VmbAPI::FramePtr fptr;
	err = camera -> AcquireSingleImage (fptr, 500); 
	if ( VmbErrorSuccess != err){
		std::cout << "Could not read the next frame!";
	}

	const VmbUchar_t *im;
	fptr->GetImage(im);
	unsigned w=640,h=480;
	fptr->GetHeight(h);
	fptr->GetWidth(w);

	/////////////////////////////////////////
	// Here comes the OpenCV part          //
	// Trying to convert to C++ interface  //
	/////////////////////////////////////////
	VmbPixelFormatType pxType; 
	fptr->GetPixelFormat(pxType);
	
	//Convert data to opencv
	cv::Mat img = cv::Mat((int)h, (int)w, CV_8UC1);
	img.data = (uchar *)im;

	cv::cvtColor(img, *image, CV_BayerRG2BGR);

	return Frame<cv::Mat>(image,timestamp,m_frameCount);

}

/*
Return true if there exist more frames available.
*/
bool FrameServerVimba::hasNext()
{
    return cameraOpened;
}


void FrameServerVimba::releaseServer()
{
    //Close the camera
	if ( VmbErrorSuccess != camera->Close() )
	{
		std::cout << "Could not close the camera" << std::endl;
	}
    cameraOpened=false;
}

bool FrameServerVimba::setExposureAuto(int n){
	AVT::VmbAPI::FeaturePtr pFeature; // Any camera feature
	VmbErrorType err;
	err = camera -> GetFeatureByName( "ExposureAuto", pFeature ); 
	err = pFeature->SetValue(n) ;
	return VmbErrorSuccess == err;
}
