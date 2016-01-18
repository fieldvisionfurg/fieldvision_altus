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
* \file VisionInterfaces.h
* \brief Implementation of generic interfaces module.
* \see Interfaces
*
* \defgroup Interfaces Interfaces
* \brief This module contains the generic interfaces used throughout the framework.
*
* The objective of these interfaces is to define minimal functionalities that detectors, trackers, categorizers, etc should implement.
*
* The image type and object is generalized by using templates, so these interfaces can be implemented for different types of
* images (e.g. OpenCv, Kinect, stereo cameras, CPL cloud points, etc) and for different types of objects (e.g. faces, cars, gestures, etc).
* 
* \ingroup Core
*/



#ifndef _VISIONINTERFACES_H
#define _VISIONINTERFACES_H

#include "VisionCore.h"

/// Contains the core functionalities of CvWorks.
/** The core functionalities should be independent of image representation.
 * It should be implemented as a header only library and should not depend on external libraries.
 */
namespace VisionCore {
/// Interfaces for generic computer vision tasks.
namespace Interfaces {



/// Interface for a source of frames. The frames can come from a video file, webcam, set of images, etc.
/** The objective of this interface is to abstract a source of frames.
 *
 * The method captureFrame() should be called to get a Frame, which contains an image, a timestamp and a frame number.
 *
 * By definition, calling this method consumes a frame from the source, so subsequent calls return the next available frame.
 *
 * 	The method hasNext() should be called before capturing a frame to verify if there is an available frame.
 *
 *  A typical use of a frame server is:

	while(frameServer.hasNext()){
        VisionCore::Frame<frameServer::ImgType> frame = frameServer.captureFrame();
		//PROCESS FRAME
	}
	frameServer.releaseServer();

	\param TImg Image type.
	\see Frame FrameServerCv AsyncFrameServerWrap
	\ingroup Interfaces
 */
template<class TImg>
class FrameServer
{
public:
    /// Captures and returns a frame from the source.
    /** This methods should only be called when method hasNext() returns true, otherwise the behaviour is undefined. */
    virtual const Frame<TImg> captureFrame() = 0;

    /// Returns true if there exist at least one frame available to be captured.
    virtual bool hasNext() = 0;

    /// Release the frame server resources (e.g. closes the video file, shut-down webcam).
    virtual void releaseServer() = 0;

	/// Destrutor.
    virtual ~FrameServer()   {   }

	typedef TImg ImgType;
};


/****************************************************************************************************/
/****************************************************************************************************/


/// Interface defining a generic object detector (e.g. faces, car, people).
/** An object detector, as defined here, is a computer vision method that finds objects in a given image.

    Usually, the object position is represented by a bounding rectangle around the object, however other representation can also be used (a bounding circle,
	the object contour, or simple a point). Some methods can detect not only the position, but also more complex information, like a skeleton 
    for a people detector.

    This interface provides a minimal and generic description of an object detector. The interface has only one method: detect(), which receives an input
	image and returns a vector of objects found within the image. 
	Both the image and object types are generic.

 \param TImg Image type used in the detector.
 \param TObj Object type returned by the detector.
 \see AsyncDetectorWrap CircleDetectorHTCF
 \ingroup Interfaces
*/
template<class TImg,class TObj>
class Detector
{
public:
    /// Detects objects in the image and returns a vector with the detected objects.
    /** The vector should be empty if no object is detected. */
    virtual std::vector<TObj> detect(const TImg& img) const = 0;

    virtual void detect(const TImg& img,std::vector<TObj>& obj) const {
        for(TObj d : detect(img))
            obj.push_back(d);
    }

	/// Destrutor.
    virtual ~Detector()  {  }

	typedef TImg ImgType;
	typedef TObj ObjType;
};


/// Interface defining a generic object tracker.
/** A tracker, as defined here, is a computer vision method that can follow an object within a video.

    Usually, the object	is represented by a bounding rectangle around the object, however other representation can also be used (a bounding circle,
	the object contour, or simple a point).

    This interface provides a generic description for a object tracker. We assume the tracker is initialized with a known position when constructed
    or is able to self-initialization. Then,	function update() is called by the user to update the position for subsequent frames.

	At any time, the method getLastTrack() can be called to get the current estimated object position.

    Function lostTrack() can be used to verify if the tracker lost track from the object, which can be caused by
    the object going out of image limits, becoming ocluded, or even due to a tracker error.

	\param TImg Image type used in the tracker.
	\param TObj Object type returned by the tracker.
	\see AsyncTrackerWrap CircleTrackerHTCF
	\ingroup Interfaces
*/
template<class TImg,class TObj>
class Tracker
{
public:
	/// Constructor method.
    Tracker() {m_lostTrack=false;}

    /// Returns a reference to the current estimated object state.
    /** This function is often call after calling the update() method.
        The returned reference is usualy to a class member.
    */
    virtual const TObj& getLastTrack() = 0;

    /// Returns true if the tracker lost the object track.
    /** The cause for loosing the object track is not specified by the interface. It can be caused by
    the object going out of image limits, becoming ocluded, or even due to a tracker error.
    */
    virtual bool lostTrack() const { return m_lostTrack;}

    /// Updates the object position (state) given a new frame.
    /** To continuously track an object throughout a video, this method should be call in sequence for all frames.*/
    virtual void update(const Frame<TImg>& frame) = 0;

	/// Destructor method.
    virtual ~Tracker() {  }

	typedef TImg ImgType;
	typedef TObj ObjType;

protected:
    /// Set whether the tracker has lost track or not.
    virtual void lostTrack(bool lost) {m_lostTrack=lost;}

	bool m_lostTrack;
};


/****************************************************************************************************/
/****************************************************************************************************/


/// A IdentifiedMultiTracker is a tracker for multiple objects, each having its own ID.
/** While the Tracker interface defines that only one object is tracked, this interface assumes multiple objects
 * are tracked at the same time.
 *
 * To track more than one object at the same time (for example, tracking multiple faces), one solution is to implement
 * interface Tracker<TImg,std::vector<TObj>>. With this approach, there is no distinction between objects, which is ok
 * for some applications but inadequate for others.
 *
 * For example, a multiple person tracker should provide an ID for each tracked person that is consistent along the video,
 * and not only an unordered vector of persons.
 *
 * The IdentifiedMultiTracker interface defines a tracker that tracks multiple objects where each object has a distinct
 * ID. This tracker provides a std::map<long,TObj> where each object has a unique key.
 *
 \see Tracker AbstractAutoTracker
    \param TImg Image type used in the tracker.
    \param TObj Object type returned by the tracker.
 \ingroup Interfaces

*/
template<class TImg,class TObj>
class IdentifiedMultiTracker : public Tracker<TImg,std::map<long,TObj>>
{
public:
    /// Destructor method.
    virtual ~IdentifiedMultiTracker() { }
};


/****************************************************************************************************/
/****************************************************************************************************/


/// Interface defining a generic image categorizer (classifier).
/** A image categorizer (or classifier) is capeable of identifing the category of a given image.
 *
 *  For example, a binary classifier could be able to classify an image as being a forest or city.
 *
 * The interface defines only one method: categorize(), which receives an input image and returns the
 * category. The default category type is std::string, but it can be specifyied (for example as bool for
 * binary classifiers).

 \param TIn Image type.
 \param TObj Category type. Default is std::string.
 \ingroup Interfaces
*/
template<class TIn,typename TCtg = std::string>
class Categorizer
{
public:
    /// Process an input sample and outputs its category.
    virtual TCtg categorize(const TIn& sample) const = 0;

    /// Destructor.
	virtual ~Categorizer() { }

	typedef TIn InType;
	typedef TCtg CtgType;
};


/****************************************************************************************************/
/****************************************************************************************************/


} //namespace interfaces
using namespace VisionCore::Interfaces;
} //namespace VisionCore

#endif // ndef _VISIONINTERFACES_H
