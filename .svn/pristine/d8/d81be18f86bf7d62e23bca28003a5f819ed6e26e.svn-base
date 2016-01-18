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

#ifndef DETECTORCONTROL_H
#define DETECTORCONTROL_H

#include "VisionCore.h"
#include "FrameServerControlWidget.h"
#include "ProcessWidget.h"
#include "ProcessControl.h"
#include "TrackerControl.h"
#include <QPushButton>
#include <qpainter.h>
#include <qpen.h>

/// Esta classe é responsável por controlar a execução de um detector.
/** Um detector deve ser passado no construtor. Este detector é executado em um thread separado
 * utilizando a classe AsyncDetectorWrap.
 *
 * Além disso, também é criado um logger que mantém registro de todos as detecções.
 *
 * \ingroup VisionGUI
 */
template<class TImg,class TObj>
class DetectorControl
        : public ProcessControl
{
public:
    DetectorControl(VisionCore::Detector<TImg,TObj>* detector_,FrameServerControlWidget* frameServerControl_);

    virtual void pause();

    virtual void start();

    virtual bool exportLog(std::string fileName){
        return loggerDetector->writeToFileAsCsv(fileName);
    }

    /// Adiciona um botão a interface gráfica que possibilita a criação de um rastreador baseado no detector.
    void addCreateTrackerOption();

    virtual ~DetectorControl();

protected:
    VisionCore::Detector<TImg,TObj>* detector;
    VisionCore::AsyncDetectorWrap<TImg,TObj>* asyncDetector;
    VisionCore::DetectorLogger<TImg,TObj>* loggerDetector;

    // FunÃ§Ã£o que desenha resultados
    virtual void drawResults(QPainter* painter);
    virtual void drawObject(QPainter* painter,const TObj& object);

    virtual void resetLog();

private:
    virtual void createDetectorBasedTracker();

    // ProcessControl interface
public:
    virtual void sync(bool s);
};


template<class TImg, class TObj>
DetectorControl<TImg,TObj>::DetectorControl(VisionCore::Detector<TImg,TObj>* detector_,FrameServerControlWidget* frameServerControl_)
    :ProcessControl(frameServerControl_)
    ,detector(detector_)
    ,loggerDetector(NULL)
    ,asyncDetector(NULL)
{
    // Creates detector logger
    loggerDetector = new VisionCore::DetectorLogger<TImg,TObj>(detector);

    // Creates asynchronous detector
    asyncDetector = new VisionCore::AsyncDetectorWrap<TImg,TObj>(frameServerControl->getAsyncFrameSrv(),loggerDetector);
    this->start();
}

template<class TImg, class TObj>
void DetectorControl<TImg,TObj>::pause()
{
    asyncDetector->stop();
}

template<class TImg, class TObj>
void DetectorControl<TImg,TObj>::start()
{
    asyncDetector->start();
}

template<class TImg, class TObj>
void DetectorControl<TImg,TObj>::drawResults(QPainter *painter)
{
    // Set pen style
	QPen paintpen(painter->pen());
	paintpen.setWidth(3);
	paintpen.setColor(this->getDrawColor());
	painter->setPen(paintpen);


    // Get data from logger
    loggerDetector->lockData();
    const std::vector<VisionCore::DetectionData<TObj>>& data = loggerDetector->getData();

    // Show status
    this->getWidget()->setProcTime(loggerDetector->averageDuration().count());
    if(asyncDetector->started())
        this->setStatus(ProcessWidget::tr("Detecting"));
    else
        this->setStatus(ProcessWidget::tr("Paused"));


    int drawLogSize = this->getWidget()->getDrawSize();
    // Draw last results
    for (int i = 1 ;i<=drawLogSize;i++){
        int index = data.size()-i;
        if(index<0)
            break;
        if(i!=1)
            painter->setOpacity(1-(float)i/drawLogSize);
        const std::vector<TObj>& result = data.at(index).result;

        // Draw results
        for(const TObj &d : result){
            this->drawObject(painter,d);
        }
    }

    loggerDetector->unlockData();
}

template<class TImg, class TObj>
void DetectorControl<TImg,TObj>::resetLog()
{
    this->loggerDetector->resetLog();
}

template<class TImg, class TObj>
void DetectorControl<TImg,TObj>::addCreateTrackerOption()
{
    QPushButton* btn = new QPushButton(ProcessWidget::tr("Create tracker"));
    QObject::connect(btn,&QPushButton::clicked,[&](){
        this->createDetectorBasedTracker();
    });
    this->getWidget()->addWidget(btn);
}

template<class TImg, class TObj>
void DetectorControl<TImg,TObj>::createDetectorBasedTracker()
{
    this->pause(); //pause detector
    VisionCore::DetectorBasedMultiTracker<TImg,TObj>* tracker = new VisionCore::DetectorBasedMultiTracker<TImg,TObj>(detector);

    //Cria controlador (tracker + async + widget)
    TrackerControl<TImg,std::map<long,TObj>>* trackControl = new TrackerControl<TImg,std::map<long,TObj>>(tracker,frameServerControl);
    trackControl->setName("Tracker based on " + this->getName());

    //Adiciona widget de controle
    this->getWidget()->addWidget(trackControl->getWidget());
}

template<class TImg, class TObj>
void DetectorControl<TImg,TObj>::drawObject(QPainter *painter, const TObj &object)
{
    ImageViewerCv::draw(painter,object); //ATENÇÃO: esta função estática precisa estar implementada para o tipo TObj
}

template<class TImg, class TObj>
void DetectorControl<TImg,TObj>::sync(bool s)
{
    if(asyncDetector)
        asyncDetector->sync(s);
}

template<class TImg, class TObj>
DetectorControl<TImg,TObj>::~DetectorControl()
{
    delete asyncDetector;
    delete loggerDetector;
    delete detector;
}


#endif // DETECTORCONTROL_H
