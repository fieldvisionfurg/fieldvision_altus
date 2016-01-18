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

#ifndef _OBJECTTRACKERCCCV_H
#define _OBJECTTRACKERCCCV_H
#include "VisionImplementationCv.h"

namespace Viscv
{

/// Simple tracker using OpenCv cascade classifier detector.
/** 
    A detector is used to search the object around the last
    tracked position.
    
    Similar to tracking by using a detector on every frame,
    but searches only on a reduced region around previous
    position. 

	\ingroup Viscv
	*/
class ObjectTrackerCCCv
    : public VisionCore::Tracker<cv::Mat,cv::Rect>
{
private:
    /// Cascade classifier used to detect objects.
    std::shared_ptr<cv::CascadeClassifier> m_cascadePtr;

    /// Stores the tracked rectangle.
    cv::Rect m_currentTrack;

    /// Used to define the maximum search area around the current tracked rectangle.
    /** Defines how many times the current rectangle will be enlarged, which will be the maximum
        search area. */
    const double m_incROI;

    /// Used to define the minimum search area around the current tracked rectangle.
    /** Defines how many times the current rectangle will be enlarged, which will be the maximum
        search area. */
    double m_redROI;

private:
    static cv::Rect& resizeRect(cv::Rect& rectangle, const double scale);

public:
	/// Default constructor.
	ObjectTrackerCCCv();

    /// Constructor method.
    ObjectTrackerCCCv(std::shared_ptr<cv::CascadeClassifier> cascadePtr,
                      const cv::Rect& initialObj);

    /// Constructor method.
    ObjectTrackerCCCv(const std::string& cascadeFile,
                      const cv::Rect& initialObj);

    /// Destructor method.
    virtual ~ObjectTrackerCCCv();

    const cv::Rect& getLastTrack();

    /// Given an image (i.e. video frame), update the tracked objects.
    void update(const VisionCore::Frame<cv::Mat>& frame);

	/// Reset tracker to a given initial state.
	void reset(const cv::Rect& rectangle);
};

}

#endif // ndef _OBJECTTRACKERCCCV_H
