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
* \file VisionAbstractions.h
* \brief Implementation of Abstraction module of CvWorks.
* \see Abstractions
*
* \defgroup Abstractions Abstractions
* \brief Contains abstract computer vision methods.
*
* This module contains several classes that provide general methods implementations for computer vision methods and
* higher level applications, such as particle filtering, detector based tracking, loggers, etc.
* The classes implemented in this module use template generalization for the image and object types.
* This way, the methods can be used with images from any source (e.g. OpenCv, Kinect,
* stereo cameras, termal cameras, etc).
* It can support diferent object representation as well (e.g. rectangles, contours, specific structs, etc).
*
* \ingroup Core
*/


#ifndef _VISIONABSTRACTIONS_H
#define _VISIONABSTRACTIONS_H

#include "VisionCore.h"

namespace VisionCore {
/// Generic computer vision methods and functionalities.
namespace Abstractions {

/// Implements a tracker that can automatically detect and track multiple objects by creating a individual tracker for each object.
/** 
 * This tracker uses a detector to periodically detect new objects (the default detection period is every 2 seconds).
 *
 * When new objects are detected, the createTracker function is called to create a new individual tracker, which is added
 * to a pool of active trackers. These trackers are updated every new frame. A tracker is  removed from the active
 * tracker pool when its lostTrack method returns true.
 * The createTracker function must be implemented by derived classes.
 *
 * The Hungarian algorithm is applied to match detected and tracked objects.
 *
 * \see IdentifiedMultiTracker
 * \param TImg The type of input image.
    \param TObj The type of object being tracked.
    \ingroup Abstractions

*/
template<class TImg,class TObj>
class AbstractAutoTracker
    : public IdentifiedMultiTracker<TImg,TObj>
{

public:
    virtual const std::map<long,TObj>& getLastTrack();

    virtual void update(const Frame<TImg>& frame);

    /// Default constructor method. No new detections will be made as the detector pointer will be NULL.
    AbstractAutoTracker();

    /// Constructor method.
    AbstractAutoTracker(Detector<TImg,TObj>* detector);

	/// Destructor method.
    virtual ~AbstractAutoTracker() {  }

protected:

    /// Object detector used to periodically find new objects.
    Detector<TImg,TObj>* m_detector;

    /// List with trackers and their respective IDs (each tracker is assumed to track a unique object).
    std::list<std::pair<long,Tracker<TImg,TObj>*>> m_trackers;

    /// Time reference (in miliseconds) of last detection.
    double m_lastDetectionTime;

    /// Time interval (in miliseconds) for running new detection.
    double m_detectPeriod;

    /// Current tracked objects. This is the tracker output.
    std::map<long,TObj> m_objects;

private:
    /// Internal counter for identifying new trackers.
	long m_trackerCount;

    /// Creates a new tracker given a initial detected object.
    virtual Tracker<TImg,TObj>* createTracker(const TObj& initialObj) = 0;

    /// Updates the list of trackers given a list of detected objects. Creates new trackers or remove them if necessary.
    virtual void updateTrackerList(const std::vector<TObj>& detObj);
};

template<class TImg,class TObj>
AbstractAutoTracker<TImg,TObj>::AbstractAutoTracker(Detector<TImg,TObj>* detector)
    : m_trackers()
    , m_detector(detector)
    , m_lastDetectionTime(-100000.0)
    , m_detectPeriod(2000.0) // dois segundos
    , m_objects()
    , m_trackerCount(0)
{

}

template<class TImg,class TObj>
AbstractAutoTracker<TImg,TObj>::AbstractAutoTracker()
    : m_trackers()
    , m_detector(NULL)
    , m_lastDetectionTime(-100000.0)
    , m_detectPeriod(2000.0) // dois segundos
    , m_objects()
    , m_trackerCount(0)
{

}

template<class TImg,class TObj>
const std::map<long,TObj>& AbstractAutoTracker<TImg,TObj>::getLastTrack()
{
	m_objects.clear();
	// Gather the results from all current trackers.
    for (std::pair<long,Tracker<TImg,TObj>*>& t : m_trackers){
		if(t.second!=NULL){
			// Creates a pair with trackerID and its result
			TObj obj=t.second->getLastTrack();
			std::pair<long,TObj> p(t.first,obj);
            m_objects.insert(p);
		}
	}
    return m_objects;
}

template<class TImg,class TObj>
void AbstractAutoTracker<TImg,TObj>::update(const Frame<TImg>& frame)
{
    // Update each tracker and check if it lost track,
    std::list<std::pair<long,Tracker<TImg,TObj>*>> trackersToRemove;
	for (std::pair<long,Tracker<TImg,TObj>*> t : m_trackers){
        t.second->update(frame);
        if(t.second->lostTrack()){
			trackersToRemove.push_back(t);
		}
    }
	// delete trackers
    //TODO: delete trackers only if lost track for x consecutive frames.
	for (std::pair<long,Tracker<TImg,TObj>*> t : trackersToRemove){
		delete t.second;
		m_trackers.remove(t);
	}
    
    // If it is time, detect new objects
    if(m_detector!=NULL && ((frame.getTimestamp()-m_lastDetectionTime) >= m_detectPeriod )){
         std::vector<TObj> detObj;
         m_detector->detect(frame.getImg(),detObj);
         m_lastDetectionTime=frame.getTimestamp();
         this->updateTrackerList(detObj);
    }
}

template<class TImg,class TObj>
void AbstractAutoTracker<TImg,TObj>::updateTrackerList(const std::vector<TObj>& detObj)
{
    const std::map<long,TObj>& curObj = this->getLastTrack();

    const unsigned int sizeD = detObj.size(); // detection size
    const unsigned int sizeT = curObj.size(); // tracking size
    if(sizeD==0){
        //REMOVE TRACKERS????
        return;
    }
    if(sizeT==0){
        //new detected object
        for(unsigned int i=0;i<sizeD;i++){
            // New detected object. Create tracker.
            Tracker<TImg,TObj>* t = this->createTracker(detObj[i]);
            m_trackerCount++;
            // Creates a pair with trackerID and tracker pointer and adds it to the trackers list.
            std::pair<long,Tracker<TImg,TObj>*> p(m_trackerCount,t);
            m_trackers.push_back(p);
        }
        return;
    }

    // Computes similarity between detections and active tracked objects
    int countD=0; // detection count
    int countT=0;// tracking count
    double* S = new double[sizeD*sizeT];  // matrix with similarities between detections and trackings
    std::vector<long> trackIDs(sizeT);
    for(const TObj& d : detObj){
        countT=0;
        for(const std::pair<long,TObj>& p : curObj ){
            if(countD==0)
                trackIDs[countT]=p.first;
            else
                assert(p.first==trackIDs[countT]);
            double similarity = VisionHelpper::similarity(d,p.second);
            S[countD*sizeT+countT]=similarity;
            countT++;
        }
        countD++;
    }

    // Solve assignment problem using Hungarian algorithm
    /* Por exemplo, A=[2 -1 0] significa que a detecção 0 foi casada ao rastreio 2, a detecção 1 não teve casamento
    e a detecção 3 foi casada ao rastreio 0*/
    std::vector<int> A = DetectorEvaluator<TImg,TObj>::computeAssignmentOptimal(S,sizeD,sizeT);

    // Remove currently tracked objects that where not detected????


    // Update tracked objects and add new detected objects
    for(unsigned int i=0;i<sizeD;i++){
        if(A[i]==-1){
            // New detected object. Create tracker.
            Tracker<TImg,TObj>* t = this->createTracker(detObj[i]);
            m_trackerCount++;
            // Creates a pair with trackerID and tracker pointer and adds it to the trackers list.
            std::pair<long,Tracker<TImg,TObj>*> p(m_trackerCount,t);
            m_trackers.push_back(p);
        }
    }
}


/**********************************************************************************************/
/**********************************************************************************************/

/// A simple tracker that applies a detector on every frame.
/**
 * This tracker does a full detection on every frame.
 * The result is the vector of detected objects.
 * \ingroup Abstractions
*/
template<class TImg,class TObj>
class DetectorBasedTracker
    : public Tracker<TImg,std::vector<TObj>>
{

protected:
    /// Stores the current tracked objects. Returned by method getLastTrack().
    std::vector<TObj> m_currentTrack;

    /// Detector used on every frame.
    Detector<TImg,TObj>* m_detector;

public:
    /// Constructor method.
    DetectorBasedTracker(Detector<TImg,TObj>* detector);

    /// Given an image (i.e. video frame), update the tracked object.
    virtual void update(const Frame<TImg>& frame);

    virtual const std::vector<TObj>& getLastTrack() { return m_currentTrack; }

    /// Destructor method.
    virtual ~DetectorBasedTracker() {  }
};


template<class TImg,class TObj>
DetectorBasedTracker<TImg,TObj>::DetectorBasedTracker(Detector<TImg,TObj>* detector )
    : m_detector(detector)
    , m_currentTrack()
{

}

template<class TImg,class TObj>
void DetectorBasedTracker<TImg,TObj>::update(const Frame<TImg>& frame)
{
    // Detects object in the image
    m_currentTrack = m_detector->detect(frame.getImg());
}

/**********************************************************************************************/
/**********************************************************************************************/

/// A tracker that applies a detector on every frame.
/**
 * This tracker does a full detection on every frame and match each detected object with the currently tracked objects.
 *
 * If a detected object is not matched with a current tracked object it is assumed that it is a new object and, therefore, it is added to the tracked objects.
 * On the other hand, if a currently tracked object is not detected in the frame, it is removed from tracked objects.
 *
 * The Hungarian algorithm is applied to match detected and tracked objects.
 *
 * \ingroup Abstractions
*/
template<class TImg,class TObj>
class DetectorBasedMultiTracker
    : public IdentifiedMultiTracker<TImg,TObj>
{

protected:
    /// Stores the current tracked objects. Returned by method getLastTrack().
    std::map<long,TObj> m_currentTrack;

	/// Detector used on every frame.
    Detector<TImg,TObj>* m_detector;

    /// Internal counter for identifying new tracked objects.
    long m_trackCount;

public:
    /// Constructor method.
    DetectorBasedMultiTracker(Detector<TImg,TObj>* detector);

    /// Given an image (i.e. video frame), update the tracked object.
    void update(const Frame<TImg>& frame);

    const std::map<long,TObj>& getLastTrack() { return m_currentTrack; }

    bool lostTrack() const { return this->m_lostTrack; }

    /// Destructor method.
    virtual ~DetectorBasedMultiTracker() {  }
};


template<class TImg,class TObj>
DetectorBasedMultiTracker<TImg,TObj>::DetectorBasedMultiTracker(Detector<TImg,TObj>* detector )
    : m_detector(detector)
    , m_trackCount(0)
    , m_currentTrack()
{

}

template<class TImg,class TObj>
void DetectorBasedMultiTracker<TImg,TObj>::update(const Frame<TImg>& frame)
{
    // Detects object in the image
    std::vector<TObj> det = m_detector->detect(frame.getImg());

    const unsigned sizeD = det.size(); // detection size
    const unsigned sizeT = m_currentTrack.size(); // tracking size
    if(sizeD==0){
        m_currentTrack.clear();
        return;
    }
    if(sizeT==0){
        //new detected object
        for(unsigned int i=0;i<sizeD;i++){
            m_trackCount++;
            m_currentTrack.insert(std::make_pair(m_trackCount,det.at(i)));
        }
        return;
    }
    
    // Computes similarity with all active tracked objects
    int countD=0; // detection count
    int countT=0;// tracking count
    double* S = new double[sizeD*sizeT];  // matrix with similarities between detections and trackings
    std::vector<long> trackIDs(sizeT);
    for(const TObj& d : det){
        countT=0;
        for(const std::pair<long,TObj>& p : m_currentTrack ){
            if(countD==0)
                trackIDs[countT]=p.first;
            else
                assert(p.first==trackIDs[countT]);
            double similarity = VisionHelpper::similarity(d,p.second);
            S[countD*sizeT+countT]=similarity;
            countT++;
        }
        countD++;
    }

    // Solve assignment problem using Hungarian algorithm
    /* Por exemplo, A=[2 -1 0] significa que a detecção 0 foi casada ao rastreio 2, a detecção 1 não teve casamento
    e a detecção 3 foi casada ao rastreio 0*/
    std::vector<int> A = DetectorEvaluator<TImg,TObj>::computeAssignmentOptimal(S,sizeD,sizeT);

    // Remove currently tracked objects that where not detected.
    //TODO: delete trackers only if lost track for x consecutive frames.
    for(unsigned int i=0;i<sizeT;i++){
        if(std::find(A.begin(),A.end(),i) == A.end()){
            //tracked object i not detected.
            m_currentTrack.erase(trackIDs[i]);
        }
    }

    // Update tracked objects and add new detected objects
    for(unsigned int i=0;i<sizeD;i++){
        if(A[i]==-1){
            //new detected object
            m_trackCount++;
            m_currentTrack.insert(std::make_pair(m_trackCount,det.at(i)));
        }
        else{
            //update tracked object
            long trackID=trackIDs[A[i]];
            m_currentTrack.at(trackID)=det.at(i);
        }
    }

    delete[] S;
}


/**********************************************************************************************/
/**********************************************************************************************/


/// Simple structure that stores data related to a detection (used in DetectionLogger).
template<class TObj>
struct DetectionData{

	/// Detection number (or ID)
	unsigned long detNumber;

	/// Detection output
    std::vector<TObj> result;

	/// Timestamp when detection was performed
	std::chrono::high_resolution_clock::time_point time;

	/// Processing time, in milliseconds.
	std::chrono::milliseconds duration;
};

/// Provides log functionalities for a detector. Stores results, processing times and statistics.
/**
 * This class wraps a detector and transparently provides log functionalities. When the detect function is called,
 * it calls the detect method of its internal detector and:
 * - Store the results.
 * - Compute the processing time for detection.
 * - Compute statistics (e.g. number of objects detected on average).
 * - Save the results to file
 *
 * \ingroup Abstractions
*/
template<class TImg,class TObj>
class DetectorLogger
    : public Detector<TImg,TObj>
{

protected:
	/// Detector used to detect objects.
    const Detector<TImg,TObj>* m_detector;

	/// Flag indicating if the logger should store the images.
	bool m_logImages;

	/// Internal count for detections
	mutable unsigned long m_detCount;

	/// Internal storage for detection results
	mutable std::vector<DetectionData<TObj>> m_data;

	/// Internal storage for images (used only im m_logImages is enabled)
	mutable std::map<unsigned long,TImg> m_imgs;

	/// Cummulative processing time of all detections
	mutable std::chrono::milliseconds m_cumDuration;

    /// Mutex for the log data
    mutable std::mutex m_dataMutex;
	
public:
    /// Constructor method.
    DetectorLogger(Detector<TImg,TObj>* detector,const bool logImages = false);

    /// Write the data to file as CSV. Uses method VisionHelpper::getAsCsvString(TObj x).
    /** The method VisionHelpper::getAsCsvString(TObj x) should be implemented for type TObj,
     * otherwise the object data will not be writen in the CSV file.
     */
	bool writeToFileAsCsv(const std::string fileName);

    /// Destructor method.
    virtual ~DetectorLogger() { }

	/// Returns the average processing duration.
    std::chrono::milliseconds averageDuration() const {return (m_detCount==0) ? std::chrono::milliseconds(0) : m_cumDuration/m_detCount;}

	/// Returns a vector with all detections data.
    const std::vector<DetectionData<TObj>>& getData() const { return m_data; }

    /// Clears all log data.
    void resetLog();

    /// Locks the log data.
    /** This function is used to synchronize the access to the data when other threads are
     * accessing it. Don't forget to unlock it after locking.
     */
    void lockData() const { m_dataMutex.lock();}

    /// Unlocks the log data.
    void unlockData() const {m_dataMutex.unlock();}

    std::vector<TObj> detect(const TImg& img) const;

};

template<class TImg,class TObj>
DetectorLogger<TImg,TObj>::DetectorLogger(Detector<TImg,TObj>* detector,const bool logImages)
	: m_detector(detector)
	, m_detCount(0)
	, m_logImages(logImages)
	, m_data ()
	, m_cumDuration(0)
{

}


template<class TImg,class TObj>
std::vector<TObj> DetectorLogger<TImg,TObj>::detect(const TImg& img) const
{
	using namespace std::chrono;

	//Detect objects
	high_resolution_clock::time_point tic =high_resolution_clock::now();
    std::vector<TObj> result = m_detector->detect(img);
	high_resolution_clock::time_point toc =high_resolution_clock::now();

	DetectionData<TObj> data;

	//Compute and store times
	milliseconds waitTime = duration_cast<milliseconds>(toc-tic);
	data.time = tic;
	data.duration = waitTime;
	m_cumDuration+=waitTime;
	
	//Store results
	data.result = result;
    lockData(); // multi-thread safety
	m_detCount++;
	data.detNumber=m_detCount;
	m_data.push_back(data);
	if(m_logImages){
		std::pair<unsigned long,TImg> pimg(m_detCount,img);
		m_imgs.insert(pimg);
	}
	
    unlockData();
	return result;
}

template<class TImg,class TObj>
bool DetectorLogger<TImg,TObj>::writeToFileAsCsv(const std::string fileName)
{
	// Create file
	std::ofstream file;
	file.open (fileName);
	if(!file.is_open())
		return false;

	// Write header
	file << "Number,Duration(ms),Size,Result" << std::endl;

	// Write all log data
	this->lockData();
	std::string comma = ",";
	for(DetectionData<TObj>& det : m_data){
		file << det.detNumber << comma;
        file << det.duration.count() << comma;
		file << det.result.size() << comma;
		for(TObj obj : det.result){
			file << VisionHelpper::getAsCsvString(obj) << comma;
		}
		file << std::endl;
	}
	this->unlockData();
	file.close();
	return true;

}

template<class TImg,class TObj>
void DetectorLogger<TImg,TObj>::resetLog()
{
    this->lockData();
    m_data.clear();
    this->unlockData();
}



/**********************************************************************************************/
/**********************************************************************************************/

/// Simple structure that stores data related to tracking. Used in TrackerLogger.
template<class TObj>
struct TrackingData{

	/// Tracking number (or ID)
	unsigned long trackNumber;

	/// Tracking output
	TObj result;

    /// Lost track.
    bool lostTrack;

	/// Timestamp when tracking was performed
	std::chrono::high_resolution_clock::time_point time;

	/// Tracker update processing time, in milliseconds.
	std::chrono::milliseconds updateDuration;

	/// Get last track processing time, in milliseconds.
	std::chrono::milliseconds gltDuration;

	/// Total processing time, in milliseconds.
	std::chrono::milliseconds duration;

};


/// Provides log functionalities for a Tracker. Stores results, processing times and statistics.
/** This class wraps a tracker and transparently provides log functionalities. When the updateTracker function is called, 
it calls the updateTracker method of its internal tracker and:
- Store the results.
- Compute the processing time for tracking.
- Compute statistics
- Save the results to file

\ingroup Abstractions
*/
template<class TImg,class TObj>
class TrackerLogger
    : public Tracker<TImg,TObj>
{

private:
	/// Tracker used to detect objects.
    Tracker<TImg,TObj>* m_tracker;

	/// Flag indicating if the logger should store the frames.
	bool m_logFrames;

	/// Internal count for tracking
    unsigned long m_trackCount;

	/// Internal storage for tracking results
	std::vector<TrackingData<TObj>> m_data;

	/// Internal storage for frames (used only im m_logFrames is enabled)
	std::map<unsigned long,Frame<TImg>> m_frames;

	/// Cummulative processing time of all tracking.
	std::chrono::milliseconds m_cumDuration;

    /// Mutex for the log data.
    mutable std::mutex m_dataMutex;
	
public:
    /// Constructor method.
    TrackerLogger(Tracker<TImg,TObj>* tracker,const bool logImages = false);

    /// Destructor method.
    virtual ~TrackerLogger() { }

	/// Returns the average processing duration.
	std::chrono::milliseconds averageDuration() const;

	/// Returns a vector with all tracking data.
	const std::vector<TrackingData<TObj>>& getData() const;

    /// Locks the log data.
    /** This function is used to synchronize the access to the data when other threads are
     * accessing it. Don't forget to unlock it after locking.
     */
    void lockData() const { m_dataMutex.lock();}

    /// Unlocks the log data.
    void unlockData() const {m_dataMutex.unlock();}

	/// Write the data to file as CSV
	bool writeToFileAsCsv(const std::string fileName);

    /// Clears all log data.
    void resetLog();

    void update(const Frame<TImg>& frame);

    const TObj& getLastTrack();

    bool lostTrack() const;
};

template<class TImg,class TObj>
TrackerLogger<TImg,TObj>::TrackerLogger(Tracker<TImg,TObj>* tracker,const bool logFrames)
	: m_tracker(tracker)
	, m_trackCount(0)
    , m_logFrames(logFrames)
    , m_data()
	, m_cumDuration(0)
{

}

template<class TImg,class TObj>
bool TrackerLogger<TImg,TObj>::writeToFileAsCsv(const std::string fileName)
{
	// Create file
	std::ofstream file;
	file.open (fileName);
	if(!file.is_open())
		return false;

	// Write header
    file << "Number,Duration(ms),Update_duration(ms),LostTrack,Result" << std::endl;

	// Write all log data
	this->lockData();
	std::string comma = ",";
	for(TrackingData<TObj>& track : m_data){
		file << track.trackNumber << comma;
		file << track.duration.count() << comma;
		file << track.updateDuration.count() << comma;
        if(track.lostTrack)
            file << "1" << comma;
        else
            file << "0" << comma;
		file << VisionHelpper::getAsCsvString(track.result) ;
		file << std::endl;
	}
	this->unlockData();
	file.close();
	return true;
}

template<class TImg,class TObj>
void TrackerLogger<TImg,TObj>::update(const Frame<TImg>& frame)
{
	using namespace std::chrono;

	//Update tracker
	high_resolution_clock::time_point tic =high_resolution_clock::now();
	m_tracker->update(frame);
	high_resolution_clock::time_point toc =high_resolution_clock::now();

	TrackingData<TObj> data;

	//Compute and store times
	milliseconds waitTime = duration_cast<milliseconds>(toc-tic);
	data.time = tic;
	data.updateDuration = waitTime;

	//Get result
	tic = high_resolution_clock::now();
	TObj result = m_tracker->getLastTrack();
	toc = high_resolution_clock::now();
	waitTime = duration_cast<milliseconds>(toc-tic);
	data.gltDuration = waitTime;
    data.duration = data.updateDuration+data.gltDuration;
	m_cumDuration+=data.duration;
	
	//Store results
	data.result = result;
    data.lostTrack = m_tracker->lostTrack();
    lockData();
	m_trackCount++;
	data.trackNumber=m_trackCount;
	m_data.push_back(data);
	if(m_logFrames){
        std::pair<unsigned long,Frame<TImg>> pframe(m_trackCount,frame);
        m_frames.insert(pframe);
	}
    unlockData();
}

template<class TImg,class TObj>
std::chrono::milliseconds TrackerLogger<TImg,TObj>::averageDuration() const{
    return (m_trackCount==0) ? std::chrono::milliseconds(0) : m_cumDuration/m_trackCount;
}

template<class TImg,class TObj>
const std::vector<TrackingData<TObj>>& TrackerLogger<TImg,TObj>::getData() const{
	return m_data;
}

template<class TImg,class TObj>
void TrackerLogger<TImg,TObj>::resetLog()
{
    this->lockData();
    m_data.clear();
    m_cumDuration=std::chrono::milliseconds(0);
    m_trackCount=0;
    this->unlockData();
}

template<class TImg,class TObj>
const TObj& TrackerLogger<TImg,TObj>::getLastTrack(){
    return m_data.back().result;
}

template<class TImg,class TObj>
bool TrackerLogger<TImg,TObj>::lostTrack() const{
	return m_tracker->lostTrack();
}



/**********************************************************************************************/
/**********************************************************************************************/


/// Implements a particle filtering algorithm for generic tracking models.
/**
    There are three application specific functions that 
    must be implemented by derived classes: 
    - evalObj, which computes the 'likeliness' of a given 
    object be present in the image.
    - sampleTransition, which generates a new object given 
    a current object.
    - averageObj, which performs a weighted combination 
    of several objects.
    
    \param TImg The type of input image.
    \param TObj The type of object being tracked.
	
	\ingroup Abstractions
*/
template<class TImg,class TObj>
class AbstractParticleFilteringTracker
    : public Tracker<TImg,TObj>
{

private:
    std::vector<TObj> m_particleSet1;
    std::vector<double> m_weights;
    std::vector<TObj> m_particleSet2;
    std::vector<TObj>* m_oldParticleSetPtr;
    std::vector<TObj>* m_newParticleSetPtr;
    TObj m_currentTrack;

protected:
    std::mt19937 m_randEngine;

private:
    virtual TObj averageObj(const std::vector<TObj>& objects,
                            const std::vector<double>& weights) = 0;
    virtual double evalObj(const TImg& img, const TObj& obj) = 0;
    virtual void sampleMultinomial(unsigned int* const sample,
                                   const double* const prob,
                                   const unsigned int sampDim,
                                   const unsigned int numOfTrials);
    virtual void sampleTransition(const TObj& oldObj, TObj& newObj) = 0;

protected:
    /// Constructor method.
    AbstractParticleFilteringTracker(const TObj& initialObj = TObj(),
                                     const unsigned int numParticles = 100);

public:
    /// Destructor method.
    virtual ~AbstractParticleFilteringTracker();
    virtual const TObj& getLastTrack();
    /// Given an image (i.e. video frame), update the tracked objects.
    virtual void update(const Frame<TImg>& frame);

	/// Reset the tracked object to a known state.
	virtual void reset(const TObj& initialObj);
};

/*
Constructor method.
*/
template<class TImg,class TObj>
AbstractParticleFilteringTracker<TImg,TObj>::AbstractParticleFilteringTracker(const TObj& initialObj,
                                                                              const unsigned int numParticles) 
    : m_particleSet1(std::vector<TObj>(numParticles,initialObj))
    , m_weights(std::vector<double>(numParticles,1.0/numParticles))
    , m_particleSet2(std::vector<TObj>(numParticles,initialObj))
    , m_oldParticleSetPtr()
    , m_newParticleSetPtr()
    , m_currentTrack()
    , m_randEngine((unsigned int)time(NULL))
{
    m_oldParticleSetPtr=&m_particleSet1;
    m_newParticleSetPtr=&m_particleSet2;
}



/*
Destructor method.
*/
template<class TImg,class TObj>
AbstractParticleFilteringTracker<TImg,TObj>::~AbstractParticleFilteringTracker()
{
}


template<class TImg,class TObj>
const TObj& AbstractParticleFilteringTracker<TImg,TObj>::getLastTrack()
{
    m_currentTrack=averageObj(*(this->m_newParticleSetPtr),m_weights);
    return m_currentTrack;
}


template<class TImg,class TObj>
void AbstractParticleFilteringTracker<TImg,TObj>::sampleMultinomial(unsigned int* const sample,
                                                                    const double* const prob,
                                                                    const unsigned int sampDim,
                                                                    const unsigned int numOfTrials)
{
	//for(unsigned int i=0;i<sampDim;i++)
	//	sample[i]=1;
	using namespace std;

    const unsigned int nb = sampDim;
    const double* const p = prob;
    const unsigned int t = numOfTrials; 
    unsigned int* const s = sample;
    
    unsigned int remainingTrials=t;
	
	//mt19937 eng = mt19937(time(NULL)); //random number generator
	//std::tr1::binomial_distribution<unsigned int, double> binomial();//binomial distribution
	
    //sample each bin from binomial
    for(unsigned int i=0;i<(nb-1);++i){
        if(remainingTrials>0){
			binomial_distribution<unsigned int> binomial(remainingTrials,p[i]);
			const unsigned int ts=binomial(this->m_randEngine);
			s[i]=ts;
			remainingTrials-=ts;
        }
		else{
			s[i]=0;
		}
    }
	s[nb-1]=remainingTrials;  //set the last bin
	assert(remainingTrials>=0);

}

/*
Given an image (i.e. video frame), update the tracked objects.
*/
template<class TImg,class TObj>
void AbstractParticleFilteringTracker<TImg,TObj>::update(const Frame<TImg>& frame)
{

    const TImg& img = frame.getImg();
    const unsigned int numOfParticles = m_newParticleSetPtr->size();

	// particulas atuais se tornam antigas
    std::swap(m_oldParticleSetPtr,m_newParticleSetPtr);
    
    //SELECT PARTICLES RAMDOMLY ACCORDING TO WEIGHTS
    unsigned int *sel = new unsigned int[numOfParticles];  //conterá a quantidade de vezes que uma particula foi selecionada
    //this->sampleMultinomial(sel,&m_weights[0],numOfParticles,numOfParticles);

	//SELECT PARTICLES PROPORTIONAL TO WEIGHTS
	double cumSteps=0.0;
	const double step=1.0/numOfParticles;
	unsigned int currentPart=0;
	sel[currentPart]=0;
	double cumWeights=m_weights[currentPart];
    for(unsigned int i=0;i<numOfParticles;i++){ //for each new particle
		cumSteps+=step;
		while(cumWeights<cumSteps && currentPart<(numOfParticles-1)){
			currentPart+=1;
			sel[currentPart]=0;
			cumWeights+=m_weights[currentPart];
		}
		sel[currentPart]+=1;
	}

    //UPDATE PARTICLES BY SAMPLING TRANSITION
	double weightSum=0.0;
	double maxWeight = std::numeric_limits<double>::min();
	double minWeight = std::numeric_limits<double>::max();
    unsigned int partCount=0;
    //#pragma omp parallel for 
	//shared(m_oldParticleSetPtr,m_newParticleSetPtr,m_weights,weightSum,partCount)
    for(unsigned int i=0;i<numOfParticles;i++){
        for(unsigned int j=0;j<sel[i];j++){
			//sample particle i a total of sel[i] times
            this->sampleTransition(m_oldParticleSetPtr->at(i),m_newParticleSetPtr->at(partCount));
			//update weiths for new sample
            m_weights[partCount]=this->evalObj(img,m_newParticleSetPtr->at(partCount));

			//find sum, min and max
		    weightSum+=m_weights[partCount];
			if(m_weights[partCount]>maxWeight)
				maxWeight=m_weights[partCount];
			if(m_weights[partCount]<minWeight)
				minWeight=m_weights[partCount];
            partCount++;
        }
        if(partCount==numOfParticles)
            break;
    }

	//normalize weights
	for(unsigned int i=0;i<m_newParticleSetPtr->size();i++){

		m_weights[i]/=weightSum; // make sum of weights = 1
		//m_weights[i]=(m_weights[i]-minWeight)/(maxWeight-minWeight); //min-max normalization
		//m_weights[i]=((m_weights[i]-minWeight)/(maxWeight-minWeight))/(weightSum-numOfParticles*minWeight);//min-max normalization and make sum of weights = 1

	}
}

/// Reset the tracked object to a known state.
template<class TImg,class TObj>
void AbstractParticleFilteringTracker<TImg,TObj>::reset(const TObj& initialObj){
	const int numParticles = m_particleSet2.size();

	//Reset particles
	m_particleSet1 = std::vector<TObj>(m_particleSet2.size(),initialObj);
	m_newParticleSetPtr=&m_particleSet1;
	m_oldParticleSetPtr=&m_particleSet2;

	//Reset weights
	m_weights = std::vector<double>(numParticles,1.0/numParticles);

	//Reset current track
	m_currentTrack=initialObj;
    this->m_lostTrack=false;
}

/// This class defines some generic helper functions that can be overrided to provide key functionalities.
/** Whithin the framework, some functions may be needed for generic data types. Like for example, converting a generic object into
  * string.
  *
  * This class provides static functions that implement such functions.
  * The user should override this functions for specific data types if necessary.

 \ingroup Interfaces
*/
class VisionHelpper
{
public:
    template<class T>
    static std::string getAsCsvString(const T& obj){
        //the default implementation do nothing.
        return "";
    }

    template<class T1, class T2>
    static double similarity(const T1& obj1,const T2& obj2){
        //the default implementation do nothing.
        return 0.0;
    }
};

} //namespace Abstractions
using namespace VisionCore::Abstractions;
} //namespace VisionCore

#endif // ndef _VISIONABSTRACTIONS_H
