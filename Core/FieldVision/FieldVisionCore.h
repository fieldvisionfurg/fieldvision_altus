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


#ifndef FILDVISIONCORE
#define FILDVISIONCORE

#include "VisionCore.h"


//template<class TImg,class TObj>
//using VisionSensor = VisionCore::Tracker;

///// Interface for a Vision Based Measurement (VBM) sensor.
///** Describe methods.
// *
// */
//template<class TImg,class TOut>
//class VisionSensor
//{
//public:
//    virtual void update(const TImg& img) = 0;

//    virtual TOut measure() = 0;

//    virtual bool reliable() = 0;

//};


/// Filters the output of a sensor (traker) using particle fileter method.
/** This class wraps a sensor (tracker) and applies a particle filter method
 * on the output.
 *
 * \ingroup FieldVision
 */
template<class TImg>
class PartFiltSensorDoubleWrap:
        public VisionCore::AbstractParticleFilteringTracker<TImg,double>
{

public:
    /// Constructor.
    PartFiltSensorDoubleWrap(VisionCore::Tracker<TImg,double>* sensor,double standDev = 1.0);

    virtual ~PartFiltSensorDoubleWrap();

    // Tracker interface
    virtual void update(const VisionCore::Frame<TImg>& frame);
    bool lostTrack() const;

    // Accessors
    double getStdDeviation() const;
    void setStdDeviation(double value);

    bool getInitialized() const;
    void setInitialized(bool value);

private:
    VisionCore::Tracker<TImg,double>* sensor;
    double rawMeasure;
    double filteredMeasure;
    double stdDeviation;
    bool initialized;
    bool ownSensor;
    std::normal_distribution<double> gaussian;

    // AbstractParticleFilteringTracker interface
    virtual double averageObj(const std::vector<double>& measures,
                            const std::vector<double>& weights);

    virtual double evalObj(const TImg& img, const double& obj);

    virtual void sampleTransition(const double& oldObj, double& newObj);

};

template<class TImg>
PartFiltSensorDoubleWrap<TImg>::PartFiltSensorDoubleWrap(VisionCore::Tracker<TImg, double> *sensor, double standDev)
    :VisionCore::AbstractParticleFilteringTracker<TImg,double>(0.0)
    ,sensor(sensor)
    ,stdDeviation(standDev)
    ,gaussian(0.0,standDev)
    ,initialized(false)
    ,ownSensor(true)
{
    this->sensor=sensor;
    rawMeasure=sensor->getLastTrack();
}

template<class TImg>
PartFiltSensorDoubleWrap<TImg>::~PartFiltSensorDoubleWrap()
{
    if(sensor && ownSensor)
        delete sensor;
}

template<class TImg>
void PartFiltSensorDoubleWrap<TImg>::update(const VisionCore::Frame<TImg> &frame)
{

    sensor->update(frame);
    rawMeasure=sensor->getLastTrack();
    //updates particle filt
    if(!sensor->lostTrack()){
        if(initialized){
            VisionCore::AbstractParticleFilteringTracker<TImg,double>::update(frame);
        }
        else{
            this->reset(rawMeasure);
            initialized=true;
        }
    }
}

template<class TImg>
bool PartFiltSensorDoubleWrap<TImg>::lostTrack() const {
    return sensor->lostTrack() || !initialized;
}

template<class TImg>
double PartFiltSensorDoubleWrap<TImg>::getStdDeviation() const
{
    return stdDeviation;
}

template<class TImg>
void PartFiltSensorDoubleWrap<TImg>::setStdDeviation(double value)
{
    stdDeviation = value;
    gaussian=std::normal_distribution<double>(0.0,stdDeviation);
}

template<class TImg>
bool PartFiltSensorDoubleWrap<TImg>::getInitialized() const
{
    return initialized;
}

template<class TImg>
void PartFiltSensorDoubleWrap<TImg>::setInitialized(bool value)
{
    initialized = value;
}

template<class TImg>
double PartFiltSensorDoubleWrap<TImg>::averageObj(const std::vector<double> &measures, const std::vector<double> &weights)
{
    double sum=0.0;
    for(int i =0;i<measures.size();i++)
        sum += measures[i]*weights[i];
    return sum;
}

template<class TImg>
double PartFiltSensorDoubleWrap<TImg>::evalObj(const TImg &img, const double &obj)
{
    static const float inv_sqrt_2pi = 0.3989422804014327;
    double a = (obj - rawMeasure) / stdDeviation;
    double p = inv_sqrt_2pi / stdDeviation * std::exp(-0.5f * a * a);
    return p;
}

template<class TImg>
void PartFiltSensorDoubleWrap<TImg>::sampleTransition(const double &oldObj, double &newObj)
{
    newObj=oldObj+this->gaussian(this->m_randEngine);
    //TODO: implement inertia
}

/**********************************************************************************************/
/**********************************************************************************************/

/// Filters the output of a sensor (traker) using the average of the last frames.
/** This class wraps a sensor (tracker) and applies a moving average over the output.
 *
 * The setWindSize() function can be used to change the number of previous outputs considered
 * to calculate the output mean.
 *
 * \ingroup FieldVision
 */
template<class TImg,class TObj>
class TemporalMeanSensorWrap:
        public VisionCore::Tracker<TImg,TObj>
{
public:
    /// Constructor
    TemporalMeanSensorWrap(VisionCore::Tracker<TImg,TObj>* sensor,unsigned int windowSize = 10);

    virtual ~TemporalMeanSensorWrap();

    // Tracker interface
    void update(const VisionCore::Frame<TImg>& frame);
    const TObj& getLastTrack();
    bool lostTrack() const;

    // Accessors
    unsigned int getWindSize() const;
    void setWindSize(unsigned int value);

private:
    VisionCore::Tracker<TImg,TObj>* sensor;
    unsigned int windSize;
    std::vector<TObj> buffer;
    unsigned long count;
    TObj filteredMeasure;
    bool ownSensor;

    /// Computes the average over the last windSize results.
    TObj average(const std::vector<TObj>& v) const;

};

template<class TImg,class TObj>
TemporalMeanSensorWrap<TImg,TObj>::TemporalMeanSensorWrap(VisionCore::Tracker<TImg, TObj> *sensor, unsigned int windowSize)
    :sensor(sensor)
    ,windSize(windowSize)
    ,buffer(windowSize)
    ,count(0)
    ,ownSensor(true)
{

}

template<class TImg,class TObj>
TemporalMeanSensorWrap<TImg,TObj>::~TemporalMeanSensorWrap()
{
    if(sensor && ownSensor)
        delete sensor;
}

template<class TImg,class TObj>
void TemporalMeanSensorWrap<TImg,TObj>::update(const VisionCore::Frame<TImg> &frame)
{
    sensor->update(frame);
    if(!sensor->lostTrack()){
        const TObj& rawMeasure = sensor->getLastTrack();
        buffer.at(count % windSize) = rawMeasure;
        filteredMeasure=this->average(buffer);
        count++;
    }
}

template<class TImg,class TObj>
const TObj &TemporalMeanSensorWrap<TImg,TObj>::getLastTrack()
{
    return filteredMeasure;
}

template<class TImg,class TObj>
bool TemporalMeanSensorWrap<TImg,TObj>::lostTrack() const
{
    return sensor->lostTrack();
}

template<class TImg,class TObj>
unsigned int TemporalMeanSensorWrap<TImg,TObj>::getWindSize() const
{
    return windSize;
}

template<class TImg,class TObj>
void TemporalMeanSensorWrap<TImg,TObj>::setWindSize(unsigned int value)
{
    windSize = value;
    buffer.resize(value);
}

template<class TImg,class TObj>
TObj TemporalMeanSensorWrap<TImg,TObj>::average(const std::vector<TObj> &v) const
{
    TObj result;
    for(int i=0;i<v.size();i++){
        if(i==0)
            result=v[i];
        else
            result+=v[i];
    }
    result=result/v.size();
    return result;
}


//KalmanSensorWrap

//TransmitterWrap


#endif // FILDVISIONCORE


