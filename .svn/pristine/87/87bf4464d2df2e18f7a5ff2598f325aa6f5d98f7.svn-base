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

#pragma once

#include "VisionSensors.h"

namespace VisionSense {

/// Rastreador que detecta se um objeto está presente ou não em uma determinada região.
/**
 *
 * \ingroup VisionSensors
 */
class PresenceTracker :
    public VisionCore::Tracker<cv::Mat,bool>
{
public:
	PresenceTracker(void);
	~PresenceTracker(void);

    void update(const VisionCore::Frame<cv::Mat>& frame);

	const bool& getLastTrack();

	/// Set the negative presence (lack of object) based on a reference image.
	void setNegativeReference(const cv::Mat& img);

	/// Set the positive presence (object is present) based on a reference image.
	void setPositiveReference(const cv::Mat& img);

	cv::Point topLeft;
	cv::Point bottonRight;

	cv::Scalar positiveColorLab;
	cv::Scalar negativeColorLab;

private:
	bool presence;
	
};

}
