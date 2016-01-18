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
* \file VisionAsync.h
* \brief Implementation of asynchronous execution module of CvWorks.
* \see Async
*
* \defgroup Async Async
* \brief Contains classes that execute the CvWorks methods asynchronously in separate threads.
*
* The classes in this module work as wraps that receive an object (detector, tracker, etc) and coordinate its
* execution in individual threads.
*
* \ingroup Core
*/

#ifndef _VISIONASYNC_H
#define _VISIONASYNC_H
#include "VisionCore.h"

namespace VisionCore {
/// Provides classes for running computer vision tasks in multiple threads.
namespace Async {

/// Allows the execution of a frame server asynchronously in an individual thread.
/** 
 * This class is a wrap for a FrameServer object that captures frames in a individual thread and
 * notifies other trheads the frame capture (using std conditional variables).
 * This way, other threads can access the captured frames by calling the getCurrentFrame() method.
 *
 * It is also possible to set a callback function (setCallback() method) that is executed after every frame capture.s.

\param TImg Image type.

\ingroup Async
*/
template<class TImg>
class AsyncFrameServerWrap
{
public:
	/// Constructor.
    AsyncFrameServerWrap(FrameServer<TImg>* frameServer,int framesPerSecond = -1);

    /// Destructor method.
    virtual ~AsyncFrameServerWrap();

    /// Start frame capturing in a individual thread.
    /** The frames are captured from the internal FrameServer object continuously.
     * While capturing is active, other threads are notified of a capture using the conditional variable.
	*/
    void start() { m_started = true;
                   m_threadExecCondVar.notify_all(); }

    /// Stops the capture of new frames (works as a pause).
    void stop() { m_started = false;}

    /// Indicates if the frames are being captured.
	bool started() const {return m_started;}

    /// Indicates if the video reached the end.
    bool finished() const {return m_finished;}

    /// Returns the most recent captured frame.
    std::shared_ptr<Frame<TImg>> getCurrentFrame() {return m_currentFrame;}

    /// Blocks the access to last captured frame to the trhead that called this function.
    void lockCurrentFrame() { m_currentFrameLock.lock(); }

    /// Releases the captured frames to other threads.
    void unlockCurrentFrame() { m_currentFrameLock.unlock(); }

    /// Returns a pointer to the internal thread exececuting frame capture.
    std::thread* getThreadPtr() {return &m_thread;}

    /// Returns the conditional variable used to notify other threads that a new frame was captured.
    std::condition_variable& getNewFrameCondVar() {return m_newFrameCondVar;}

    /// Sets a callback function that is executed after every frame capture.
    /** This funcion defines a callback function to be executed after the frame capture.
     *
     * To define multiple callback functions use addCallback() function instead.
    */
    void setCallback(const std::function<void(std::shared_ptr<Frame<TImg>>)>& callback) {
        m_callback.clear();
        m_callback.push_back(callback);
    }

    /// Add a callback function that is executed after each capture.
    void addCallback(const std::function<void(std::shared_ptr<Frame<TImg>>)>& callback) {
        m_callback.push_back(callback);
    }

    void addSyncMutex(std::mutex* syncMutex) {
        auto it = std::find(m_syncMutex.begin(), m_syncMutex.end(), syncMutex);
        if (it == m_syncMutex.end())
            m_syncMutex.push_back(syncMutex);
    }

    void removeSyncMutex(std::mutex* syncMutex) {
        auto it = std::find(m_syncMutex.begin(), m_syncMutex.end(), syncMutex);
        if (it != m_syncMutex.end())
            m_syncMutex.erase(it);
    }

    /// Sets how many frames will be captured. The frame server will pause(stop) after capturing these frames.
    /// Negative values means no restriction.
    void setFramesToCapture(const int quantity) { m_framesToCapture=quantity; }

    /// Sets a new frame server. The old is not deleted.
    void setFrameServer(FrameServer<TImg>* fs) { m_frameServer = fs;}
	
    /// Get the frames captured per second.
    double getFramesPerSecond() const;

    /// Sets the frames captured per second. If it is less or equal to 0 the capture will be as fast as possible (may be problematic for other threads processing the frames).
    void setFramesPerSecond(double framesPerSecond);

private:
    /// Pointer for the internal FrameServer.
    FrameServer<TImg>* m_frameServer;

    /// Thread used in FrameServer capturing.
    std::thread m_thread;

    /// Internal function executed in m_thread.
    void threadExecution();

    /// Last captured frame. Shared among several threads. You should use lockCurrentFrame and unlockCurrentFrame for synchronize access.
	std::shared_ptr<Frame<TImg>> m_currentFrame;

    /// Internal mutex for synchronizing m_currentFrame access.
	std::mutex m_currentFrameMutex;

    /// Lock for m_currentFrame.
	std::unique_lock<std::mutex> m_currentFrameLock;

    /// Mutex for synchronizing thread execution (the thread owns the mutex when executing).
    std::mutex m_thrExeMutex;

    /// Lock for synchronizing thread execution.
    std::unique_lock<std::mutex> m_thrExeLock;

    /// Signals that the frame capturing is active.
	bool m_started;

    /// Signals that the frame capturing should end.
	bool m_finish;

    /// Signals that the frame capturing ended.
	bool m_finished;

    /// Defines how many frames are captured per second. If it is less or equal to 0 the capture will be as fast as possible (may be problematic for other threads processing the frames).
    double m_framesPerSecond;

    /// Defines how many frames will be captured. The frame server will pause(stop) after capturing these frames.
    /// Negative values means no restriction.
    int m_framesToCapture;

    /// Conditional variable used to notify other threads that a new frame was captured.
	std::condition_variable m_newFrameCondVar;

    /// Conditional variable used to control thread execution.
    std::condition_variable m_threadExecCondVar;

    /// Callback functions executed after every new frame capture.
    std::vector<std::function<void(std::shared_ptr<Frame<TImg>>)>> m_callback;

    std::vector<std::mutex*> m_syncMutex;
};

/*
Constructor method.
*/
template<class TImg>
AsyncFrameServerWrap<TImg>::AsyncFrameServerWrap(FrameServer<TImg>* frameServer,int framesPerSecond)
	: m_started(false)
	, m_frameServer(frameServer)
	, m_currentFrame(NULL)
	, m_finished(false)
	, m_finish(false)
	, m_framesPerSecond(framesPerSecond)
    , m_framesToCapture(-1)
{
    m_thrExeLock = std::unique_lock<std::mutex>(m_thrExeMutex,std::defer_lock);
	m_currentFrameLock = std::unique_lock<std::mutex>(m_currentFrameMutex, std::defer_lock);
	m_thread = std::thread(std::bind(&AsyncFrameServerWrap::threadExecution,this));
}

/*
Destructor method.
*/
template<class TImg>
AsyncFrameServerWrap<TImg>::~AsyncFrameServerWrap()
{
	m_finish=true;
	m_thread.join();
}


template<class TImg>
void AsyncFrameServerWrap<TImg>::threadExecution()
{
	using namespace std::chrono;

    m_thrExeLock.lock();
	high_resolution_clock::time_point captureTime;
    milliseconds period;
    while(m_frameServer->hasNext()){
		if(m_finish)
			break;
		if( m_started){
            {
                std::vector<std::unique_lock<std::mutex>> syncLocks;
                for(std::mutex* m : m_syncMutex)
                    syncLocks.push_back(std::unique_lock<std::mutex>(*m));
                // Store time of capture
                captureTime = high_resolution_clock::now();

                // Capture frame and copy it to m_currentFrame;
                lockCurrentFrame();
                m_currentFrame = std::shared_ptr<Frame<TImg>>(new Frame<TImg>(m_frameServer->captureFrame()));
                unlockCurrentFrame();
            }

			// Callback execution passing captured frame
            for(std::function<void(std::shared_ptr<Frame<TImg>>)>& fcn : m_callback)
                fcn(m_currentFrame);
			// Signal new frame to other threads
			m_newFrameCondVar.notify_all();

			// Enforce specified frames per second (if setted > 0)
            period=milliseconds(1000/(int)m_framesPerSecond); // period between frames
			if(m_framesPerSecond>0){
				milliseconds waitTime = duration_cast<milliseconds>(period-(high_resolution_clock::now()-captureTime));
				if(waitTime.count()>0)
					std::this_thread::sleep_for(waitTime);
			}

            // If the frames to capture reached 0 then pause
            if(m_framesToCapture>0)
                m_framesToCapture--;
            if(m_framesToCapture==0){
                m_framesToCapture=-1;
                m_started=false;
            }
		}
		else{
            // Wait untill start()
            m_threadExecCondVar.wait(m_thrExeLock);
		}
	}
	m_finished=true;
    m_thrExeLock.unlock();
}

template<class TImg>
double AsyncFrameServerWrap<TImg>::getFramesPerSecond() const
{
    return m_framesPerSecond;
}

template<class TImg>
void AsyncFrameServerWrap<TImg>::setFramesPerSecond(double framesPerSecond)
{
    m_framesPerSecond = framesPerSecond;
}


/**********************************************************************************************/
/**********************************************************************************************/

/// Allows the execution of a vision method asynchronously in an individual thread.
/**
 * This is a abstract class for a processing a video asynchronously. It listens an AsyncFrameServerWrap object (given in the constructor) and, when a new frame becomes
 * available, it calls the process method, passing the image.
 *
 * After every processing, the results are passed to a user provided callback function (see setCallback).
 *
 * The method runs in a exclusive internal thread.

\param TImg Image type.
\param TOut Type returned by the processing method.

\ingroup Async
*/
template<class TImg,class TOut>
class AsyncVisionExecution
{

private:
    /// Pointer for a AsyncFrameServerWrap that will be listen.
    /** A frame is retrieved and processed when the async frame server notifies that a new frame is available.  */
    AsyncFrameServerWrap<TImg>* m_asyncFrameServerPtr;

    /// Internal thread used in detector execution.
    std::thread m_thread;

    /// Callback function executed after every processing.
    /** The result of processing is passed to this function as input variable.
     */
    std::function<void(TOut)> m_callback;

    /// Mutex for synchronizing thread execution.
    std::mutex m_thrExeMutex;

    /// Lock for synchronizing thread execution.
    std::unique_lock<std::mutex> m_thrExeLock;

    /// Signals if the processing is active (i.e. if the frames are being captured and processed).
    bool m_started;

    /// Signals that the processing should end.
    bool m_finish;

    /// Signals if the thread is working (e.g processing a frame) or idle (e.g. waiting for a frame).
    bool m_idle;

    /// Internal function executed in m_thread.
    void threadExecution();

    /// Process a frame, returning a result.
    virtual TOut process(std::shared_ptr<Frame<TImg>>) = 0;

protected:
    /// Constructor method.
    /** The user should provide a pointer to an AsyncFrameServerWrap, that will be listened for new frames.
     */
    AsyncVisionExecution(AsyncFrameServerWrap<TImg>* afsPtr);

public:
    /// Destructor method.
    virtual ~AsyncVisionExecution();

    /// Set the callback function executed after every detection.
    /** The result of processing is passed to this function as input variable.
     */
    void setCallback(const std::function<void(TOut)>& callback);

    /// Activates the processing (i.e. frames will be processed).
    void start() { m_started = true;}

    /// Stops the processing. Works as a pause.
    void stop() { m_started = false;}

    /// Indicates if the processing is active (i.e. is listening and processing frames).
    bool started() { return m_started;}

    void sync(bool s);

    /// Returns a pointer to the internal thread.
    std::thread* getThreadPtr() {return &m_thread;}
};


template<class TImg,class TOut>
AsyncVisionExecution<TImg,TOut>::AsyncVisionExecution(AsyncFrameServerWrap<TImg>* afsPtr)
    : m_asyncFrameServerPtr(afsPtr)
    , m_callback()
    , m_started(false)
    , m_finish(false)
{
    m_thrExeLock = std::unique_lock<std::mutex>(m_thrExeMutex,std::defer_lock);
    m_thread = std::thread(std::bind(&AsyncVisionExecution::threadExecution,this));
}


template<class TImg,class TOut>
AsyncVisionExecution<TImg,TOut>::~AsyncVisionExecution()
{
    m_finish=true;
    m_asyncFrameServerPtr->removeSyncMutex(&m_thrExeMutex);
    m_thread.join();
}


template<class TImg,class TOut>
void AsyncVisionExecution<TImg,TOut>::threadExecution()
{
    m_thrExeLock.lock();
    std::condition_variable& cv = m_asyncFrameServerPtr->getNewFrameCondVar();
    while(m_asyncFrameServerPtr->finished()==false && m_finish==false){
        //wait for new frame notification
        cv.wait(m_thrExeLock);
        while(!m_started && !m_finish)
            cv.wait_for(m_thrExeLock,std::chrono::seconds(1));
        if(m_finish)
            break;

        //capture and process frame
        m_asyncFrameServerPtr->lockCurrentFrame();
        std::shared_ptr<Frame<TImg>> f = m_asyncFrameServerPtr->getCurrentFrame();
        //TODO: improve performance: use read-write lock (not yet available in C++11)
        m_asyncFrameServerPtr->unlockCurrentFrame();

        try{
             TOut result = process(f);
            //call callback, passing the result
            if(m_callback!=NULL && m_callback)
                m_callback(result);
        }
        catch(std::exception& e){
            std::cout<<"Deu pau no processo. \n";
            std::cout<<e.what();
            std::cout.flush();
            m_finish=true;
        }
    }
    m_started=false;
    m_thrExeLock.unlock();
}


template<class TImg,class TOut>
inline void AsyncVisionExecution<TImg,TOut>::setCallback(const std::function<void(TOut)>& callback)
{
    m_callback = callback;
}

template<class TImg,class TOut>
void AsyncVisionExecution<TImg,TOut>::sync(bool s) {
    if(s)
        m_asyncFrameServerPtr->addSyncMutex(&m_thrExeMutex);
    else
        m_asyncFrameServerPtr->removeSyncMutex(&m_thrExeMutex);
}



/**********************************************************************************************/
/**********************************************************************************************/

/// Allows the execution of a detector asynchronously in an individual thread.
/** 
 * This class is a wrap for a Detector object. It listens an AsyncFrameServerWrap object (given in the constructor) and, when a new frame becomes
 * available, it executes the object detection.
 *
 * After every detection, the results are passed to a user provided callback function (see setCalback).
 *
 * The detector runs in a exclusive internal thread.

\param TImg Image type.
\param TObj Object type.

\ingroup Async
*/
template<class TImg,class TObj>
class AsyncDetectorWrap : public AsyncVisionExecution<TImg,std::vector<TObj>>
{

private:
    /// Internal detector.
    const Detector<TImg,TObj>* m_detector;

    /// Process a frame. Apply the detector over a frame, returning the result.
    std::vector<TObj> process(std::shared_ptr<Frame<TImg>> frame);

public:
    /// Constructor method.
    /** The user should provide a pointer to a async frame server, that will be listened for new frames, and a
     * detector, that will be executed on every captured frame.
     */
    AsyncDetectorWrap(AsyncFrameServerWrap<TImg>* afsPtr, const Detector<TImg,TObj>* detector);

    virtual ~AsyncDetectorWrap() { }

    const Detector<TImg, TObj>* getDetector() const;
    void setDetector(const Detector<TImg, TObj>* Sdetector);
};

template<class TImg,class TObj>
AsyncDetectorWrap<TImg,TObj>::AsyncDetectorWrap(AsyncFrameServerWrap<TImg>* afsPtr, const Detector<TImg,TObj>* detector)
    : AsyncVisionExecution<TImg,std::vector<TObj>>(afsPtr)
    , m_detector(detector)
{

}

template<class TImg,class TObj>
const Detector<TImg, TObj>* AsyncDetectorWrap<TImg, TObj>::getDetector() const
{
return m_detector;
}

template<class TImg,class TObj>
void AsyncDetectorWrap<TImg, TObj>::setDetector(const Detector<TImg, TObj>* detector)
{
m_detector = detector;
}

template<class TImg,class TObj>
std::vector<TObj> AsyncDetectorWrap<TImg,TObj>::process(std::shared_ptr<Frame<TImg>> frame)
{
    return m_detector->detect(frame->getImg());
}


/**********************************************************************************************/
/**********************************************************************************************/


/// Allows the execution of a tracker asynchronously in an individual thread.
/**
 * This class is a wrap for a Tracker object. It listens an AsyncFrameServerWrap object (given in the constructor) and, when a new frame becomes
 * available, it updates the tracker.
 *
 * After every tracker update, the result (i.e. current tracked object position) is passed to a user provided callback function (see setCalback).
 *
 * The tracker runs in a exclusive internal thread.

\param TImg Image type.
\param TObj Object type.

\ingroup Async
*/
template<class TImg,class TObj>
class AsyncTrackerWrap : public AsyncVisionExecution<TImg,TObj>
{

private:
    /// Callback function executed if the tracker looses track.
    std::function<void()> m_lostTrackCallback;

    /// Internal tracker.
    Tracker<TImg,TObj>* m_tracker;

    /// Process a frame. Apply the tracker over a frame, returning the result.
    TObj process(std::shared_ptr<Frame<TImg>>);

public:
    /// Constructor method.
    AsyncTrackerWrap(AsyncFrameServerWrap<TImg>* afsPtr,Tracker<TImg,TObj>* tracker);

    virtual ~AsyncTrackerWrap() { }

	/// Set the lost track callback.
    void setLostTrackCallback(const std::function<void()>& rCallback) { m_lostTrackCallback = rCallback;}

    Tracker<TImg, TObj>* getTracker() const;

    void setTracker(Tracker<TImg, TObj>* tracker);
};


template<class TImg,class TObj>
AsyncTrackerWrap<TImg,TObj>::AsyncTrackerWrap(AsyncFrameServerWrap<TImg>* afsPtr,Tracker<TImg,TObj>* tracker)
    : AsyncVisionExecution(afsPtr)
    , m_tracker(tracker)

{

}

template<class TImg,class TObj>
Tracker<TImg, TObj>* AsyncTrackerWrap<TImg, TObj>::getTracker() const
{
return m_tracker;
}

template<class TImg,class TObj>
void AsyncTrackerWrap<TImg, TObj>::setTracker(Tracker<TImg, TObj>* tracker)
{
m_tracker = tracker;
}

template<class TImg,class TObj>
TObj AsyncTrackerWrap<TImg,TObj>::process(std::shared_ptr<Frame<TImg>> frame)
{
    m_tracker->update(*frame);

    // if lost track, a callback function is called
    if(m_tracker->lostTrack()){
        if(m_lostTrackCallback!=NULL && m_lostTrackCallback)
            m_lostTrackCallback();
    }
    return m_tracker->getLastTrack();
}

/**********************************************************************************************/
/**********************************************************************************************/

/**********************************************************************************************/
/**********************************************************************************************/

/// Allows the execution of arbitrary function that processes frames in an individual thread.
/**
 * This class is a wrap for a arbitrary function that processes frames.
 * It listens an AsyncFrameServerWrap object (given in the constructor) and, when a new frame becomes
 * available, it executes the function.
 *
 * After every detection, the results are passed to a user provided callback function (see setCalback).
 *

\param TImg Image type.
\param TObj Object type.

\ingroup Async
*/
template<class TImg,class TObj>
class AsyncFunctionWrap : public AsyncVisionExecution<TImg,TObj>
{

private:
    /// Internal detector.
    const std::function<TObj(std::shared_ptr<Frame<TImg>>)> m_function;

    /// Process a frame. Delegate to function defined with setFunction().
    TObj process(std::shared_ptr<Frame<TImg>> frame);

public:
    /// Constructor method.
    /** The user should provide a pointer to a async frame server, that will be listened for new frames, and a
     * function, that will be executed on every captured frame.
     */
    AsyncFunctionWrap(AsyncFrameServerWrap<TImg>* afsPtr,  const std::function<TObj(std::shared_ptr<Frame<TImg>>)>& function);

    virtual ~AsyncFunctionWrap() { }

    void setFunction(const std::function<TObj(std::shared_ptr<Frame<TImg>>)>& function);
};

template<class TImg,class TObj>
AsyncFunctionWrap<TImg,TObj>::AsyncFunctionWrap(AsyncFrameServerWrap<TImg>* afsPtr, const std::function<TObj(std::shared_ptr<Frame<TImg>>)>& function)
    : AsyncVisionExecution<TImg,TObj>(afsPtr)
    , m_function(function)
{

}

template<class TImg,class TObj>
void AsyncFunctionWrap<TImg, TObj>::setFunction(const std::function<TObj(std::shared_ptr<Frame<TImg>>)>& function)
{
m_function = function;
}

template<class TImg,class TObj>
TObj AsyncFunctionWrap<TImg,TObj>::process(std::shared_ptr<Frame<TImg>> frame)
{
    return m_function(frame);
}



} //namespace Async
using namespace VisionCore::Async;
} //namespace VisionCore
#endif // ndef _VISIONASYNC_H
