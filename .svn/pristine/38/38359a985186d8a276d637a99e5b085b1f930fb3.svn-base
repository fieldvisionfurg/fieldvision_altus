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

#ifndef TRACKERCONTROL_H
#define TRACKERCONTROL_H

#include "VisionCore.h"
#include "FrameServerControlWidget.h"
#include "ProcessControl.h"


/// Esta classe é responsável por controlar a execução de um rastreador.
/** Um rastreador deve ser passado no construtor. Este rastreador é executado em um thread separado
 * utilizando a classe AsyncDetectorWrap.
 *
 * Além disso, também é criado um logger que mantém registro de todos os rastreios.
 *
 * \ingroup VisionGUI
 */
template<class TImg,class TObj>
class TrackerControl
        : public ProcessControl
{
public:
    TrackerControl(FrameServerControlWidget* fsControlPtr_);

    TrackerControl(VisionCore::Tracker<TImg,TObj>* trackerPtr_,FrameServerControlWidget* fsControlPtr_,bool startPaused=false);

    void setTracker(VisionCore::Tracker<TImg,TObj>* trackerPtr_,bool startPaused=false);

    virtual void pause();

    virtual void start();

    virtual bool exportLog(std::string fileName){
        return loggerTracker->writeToFileAsCsv(fileName);
    }

    virtual void resetLog();

    virtual ~TrackerControl();

    // ProcessControl interface
    virtual void sync(bool s);

protected:
    VisionCore::Tracker<TImg,TObj>* tracker;
    VisionCore::AsyncTrackerWrap<TImg,TObj>* asyncTracker;
    VisionCore::TrackerLogger<TImg,TObj>* loggerTracker;
    // Função que desenha resultados
    virtual void drawResults(QPainter* painter);
    virtual void drawObject(QPainter* painter,const TObj& object);

private:


};

template<class TImg, class TObj>
TrackerControl<TImg,TObj>::TrackerControl(FrameServerControlWidget* fsControlPtr_)
    :ProcessControl(fsControlPtr_)
    ,tracker(NULL)
    ,asyncTracker(NULL)
    ,loggerTracker(NULL)
{

}


template<class TImg, class TObj>
TrackerControl<TImg,TObj>::TrackerControl(VisionCore::Tracker<TImg,TObj>* tracker_,FrameServerControlWidget* frameServerControl_,bool startPaused)
    :ProcessControl(frameServerControl_)
    ,tracker(NULL)
    ,asyncTracker(NULL)
    ,loggerTracker(NULL)
{
    this->setTracker(tracker_,startPaused);
}


template<class TImg, class TObj>
void TrackerControl<TImg,TObj>::setTracker(VisionCore::Tracker<TImg,TObj>* trackerPtr_,bool startPaused=false)
{
    if(asyncTracker!=NULL){
        //What TODO?
        asyncTracker->stop();
        delete asyncTracker;
    }
    tracker=trackerPtr_;

    // Creates logger wrap
    loggerTracker = new VisionCore::TrackerLogger<TImg,TObj>(tracker);

    // Cria tracker assincrono
    VisionCore::AsyncFrameServerWrap<TImg>* frameSrvPtr = frameServerControl->getAsyncFrameSrv();
    asyncTracker = new VisionCore::AsyncTrackerWrap<TImg,TObj>(frameSrvPtr,loggerTracker);

    //TODO: definir o que acontece quando perde rastreio

    if(!startPaused && tracker!=NULL)
        this->start();
}


template<class TImg, class TObj>
void TrackerControl<TImg,TObj>::pause()
{
    asyncTracker->stop();
}

template<class TImg, class TObj>
void TrackerControl<TImg,TObj>::start()
{
    asyncTracker->start();
}


template<class TImg, class TObj>
void TrackerControl<TImg,TObj>::drawResults(QPainter *painter){

    // Set pen style
    QPen paintpen(painter->pen());
    paintpen.setWidth(3);
    paintpen.setColor(this->getDrawColor());
    painter->setPen(paintpen);

    // Get data from logger
    loggerTracker->lockData();
    const std::vector<VisionCore::TrackingData<TObj>>& data = loggerTracker->getData();

    // Show status
    this->getWidget()->setProcTime(loggerTracker->averageDuration().count());
    if(asyncTracker->started() && data.size()>0){
        if(data.back().lostTrack)
            this->setStatus(QString("Lost track"));
        else
            this->setStatus(QString("Tracking"));
    }
    else{
        this->setStatus(QString("Paused"));
    }


    int drawLogSize = this->getWidget()->getDrawSize();

    // Draw last results
    for (int i = 1 ;i<=drawLogSize;i++){
        int index = data.size()-i;
        if(index<0)
            break;
        if(i!=1)
            painter->setOpacity(1-(float)i/drawLogSize);
        const TObj& result = data.at(index).result;

       drawObject(painter,result);
    }

    loggerTracker->unlockData();
}

template<class TImg, class TObj>
void TrackerControl<TImg,TObj>::drawObject(QPainter *painter, const TObj &object)
{
    ImageViewerCv::draw(painter,object); //ATENÇÃO: esta função estática precisa estar implementada para o tipo TObj
}

template<class TImg, class TObj>
void TrackerControl<TImg,TObj>::resetLog()
{
    this->loggerTracker->resetLog();
}

template<class TImg, class TObj>
void TrackerControl<TImg,TObj>::sync(bool s)
{
    if(asyncTracker)
        asyncTracker->sync(s);
}

template<class TImg, class TObj>
TrackerControl<TImg,TObj>::~TrackerControl()
{
    delete asyncTracker;
    delete loggerTracker;
    delete tracker;
}



#endif // TRACKERCONTROL_H
