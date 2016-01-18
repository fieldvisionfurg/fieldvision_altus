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
/**
* \file VisionDataStructures.h
* \brief Declaration and implementation of simple data structures for Vision framework.
* \see DataStructures
*
* \defgroup DataStructures DataStructures
* \brief Declaration and implementation of simple data structures for Vision framework.
* \ingroup Core
*
* \see Interfaces Abstractions Evaluation Async
*
*/

#ifndef _VISIONDATASTRUCTURES_H
#define _VISIONDATASTRUCTURES_H

namespace VisionCore {
/// Data structures used within CvWorks.
namespace DataStructures {

/// A Frame contains an image, number and timestamp from a video sequence.
/** 
    This class contains (and owns) an image, as well as a frame number and a timestamp that relate to the time
    and position of the image within a video.

    A Frame is constructed by providing an image pointer, in which case the Frame object becomes
    the owner of the image object.
*/
template<class TImg>
struct Frame
{
private:
	/// Pointer to an image.
    std::shared_ptr<TImg> m_imgPtr;
	/// Value related to the frame time within the video. No unit is specified.
    const double m_timestamp;
    /// Frame number within the video.
    unsigned long m_number;

public:
	/// Default constructor. The image pointer will be NULL.
	Frame();

	/// Copy constructor.
	Frame(const Frame<TImg>& f);

	/// Constructor method. The Frame object becomes the owner of imgPtr.
    Frame(TImg* const imgPtr, const double timestamp = -1.0 , long number = -1);

    /// Constructor method.
    Frame(std::shared_ptr<TImg> imgPtr, const double timestamp = -1.0 , long number = -1);

    /// Destructor method.
    virtual ~Frame();

	/// Returns a reference to the frame image.
    const TImg& getImg() const {
		return *m_imgPtr;
	}

    /// Returns a pointer to the frame image.
    std::shared_ptr<TImg> const getImgPtr() const {
		return m_imgPtr;
	}

    /// Returns the frame number.
    unsigned long getNumber() const {
	    return m_number;
	}

    /// Returns the frame timestamp.
	/** The unit (e.g. miliseconds, tics) is not defined. Usually, the unit is defined within an application.
	*/
    const double getTimestamp() const {
		return m_timestamp;
	}
};


template<class TImg>
Frame<TImg>::Frame() 
    : m_imgPtr(NULL)
    , m_timestamp(0)
    , m_number(0)
{

}

template<class TImg>
Frame<TImg>::Frame(const Frame<TImg>& f) 
    : m_imgPtr(f.getImgPtr())
    , m_timestamp(f.getTimestamp())
    , m_number(f.getNumber())
{

}

template<class TImg>
Frame<TImg>::Frame(TImg* const imgPtr, const double timestamp,long number) 
    : m_imgPtr(std::shared_ptr<TImg>(imgPtr))
    , m_timestamp(timestamp)
    , m_number(number)
{

}

template<class TImg>
Frame<TImg>::Frame(std::shared_ptr<TImg> imgPtr, const double timestamp,long number)
    : m_imgPtr(imgPtr)
    , m_timestamp(timestamp)
    , m_number(number)
{

}

template<class TImg>
Frame<TImg>::~Frame()
{

}

}
using namespace VisionCore::DataStructures;
}
/**********************************************************************************************/
/**********************************************************************************************/


#endif // ndef _VISIONDATASTRUCTURES_H
