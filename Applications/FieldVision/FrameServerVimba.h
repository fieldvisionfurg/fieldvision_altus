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
#pragma once

#include "VimbaCPP\Include\VimbaCPP.h"
#include "VisionImplementationCv.h"

/// Captura frames de uma c�mera firewire da Allied Vision (Vimba).
/**
 *
 * \ingroup FieldVision
 */
class FrameServerVimba :
    public VisionCore::FrameServer<cv::Mat>
{

private:
	AVT::VmbAPI::CameraPtr camera;
	long m_frameCount;
    bool cameraOpened;

public:
	/// Captures the next frame available.
    virtual const VisionCore::Frame<cv::Mat> captureFrame();
    /// Return true if there exist more frames available.
    virtual bool hasNext();
	/// Stop capturing and release any resource (closes webcam or file).
    virtual void releaseServer();
	// Configure Exposure
	bool setExposureAuto(int n);

	FrameServerVimba(void);
	virtual ~FrameServerVimba(void);
};

