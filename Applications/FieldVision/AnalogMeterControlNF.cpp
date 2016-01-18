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
#include "AnalogMeterControlNF.h"
#include "ModbusSensorWrap.h"
#include "ModbusConfigWidget.h"
#include "FieldVisionCore.h"

AnalogMeterControlNF::AnalogMeterControlNF(FrameServerControlWidget* frameServerControl_)
    :TrackerControl(frameServerControl_)
{
    typedef double TObj;
    typedef cv::Mat TImg;

    // Cria tracker
    analogMeterTracker = new VisionSense::AnalogMeterTrackerPC();
    int imgCols = frameServerControl_->getImgViewer()->getCvImg().cols;
    int imgRows = frameServerControl_->getImgViewer()->getCvImg().rows;
    analogMeterTracker->setCenterX(imgCols/2);
    analogMeterTracker->setCenterY(imgRows/2);

    // Rastreador para posição do medidor
    Viscv::TemplMatchingTracker* posTracker = new Viscv::TemplMatchingTracker();
    analogMeterTracker->setPositionTracker(posTracker);

    // Cria filtro de média temporal
    TemporalMeanSensorWrap<TImg,TObj>* filtWrap = new TemporalMeanSensorWrap<TImg,TObj>(analogMeterTracker);

    // Wrap modbus
    ModbusSensorWrap<TImg,TObj>* modbusWrap = new ModbusSensorWrap<TImg,TObj>(filtWrap);
    ModbusConfigWidget* modbusWidget = new ModbusConfigWidget();
    modbusWidget->setup(modbusWrap);
    this->getWidget()->addWidget(modbusWidget);

    // Adiciona parâmetros a interface gráfica
    this->addLeftClickHandler("Click on center",[=](QPoint p){
        analogMeterTracker->setCenterX(p.x());
        analogMeterTracker->setCenterY(p.y()    );
    });
    //this->addNumericParameter("Center (X)",std::bind(&VisionSense::AnalogMeterTrackerPC::setCenterX,analogMeterTracker,std::placeholders::_1),0,imgCols-1,analogMeterTracker->getCenterX(),imgCols);
    //this->addNumericParameter("Center (Y)",std::bind(&VisionSense::AnalogMeterTrackerPC::setCenterY,analogMeterTracker,std::placeholders::_1),0,imgRows-1,analogMeterTracker->getCenterY(),imgRows);

    this->addNumericParameter(ProcessWidget::tr("Radius (X)")
                              ,std::bind(&VisionSense::AnalogMeterTrackerPC::setHRadius,analogMeterTracker,std::placeholders::_1)
                              ,(double)0
                              ,imgCols/2.0
                              ,analogMeterTracker->getHRadius()
                              ,imgRows);

    this->addNumericParameter(ProcessWidget::tr("Radius (Y)")
                              ,std::bind(&VisionSense::AnalogMeterTrackerPC::setVRadius,analogMeterTracker,std::placeholders::_1)
                              ,(double)0
                              ,imgRows/2.0
                              ,analogMeterTracker->getVRadius()
                              ,imgCols);

    this->addNumericParameter(ProcessWidget::tr("Tilt (degrees)")
                              ,std::bind(&VisionSense::AnalogMeterTrackerPC::setTilt,analogMeterTracker,std::placeholders::_1)
                              ,-90
                              ,90
                              ,analogMeterTracker->getTilt()
                              ,180);

    this->addNumericParameter(ProcessWidget::tr("Pointer proportion")
                              ,std::bind(&VisionSense::AnalogMeterTrackerPC::setBorderRatio,analogMeterTracker,std::placeholders::_1)
                              ,0.0
                              ,1.0
                              ,analogMeterTracker->getBorderRatio());

    this->addNumericParameter(ProcessWidget::tr("Resolution")
                              ,std::bind(&VisionSense::AnalogMeterTrackerPC::setResolution,analogMeterTracker,std::placeholders::_1)
                              ,0.0
                              ,1.0
                              ,analogMeterTracker->getResolution());

    this->addNumericParameter(ProcessWidget::tr("Pixel Intensity")
                              ,std::bind(&VisionSense::AnalogMeterTrackerPC::setMaxPointerIntensity,analogMeterTracker,std::placeholders::_1)
                              ,0
                              ,255
                              ,analogMeterTracker->getMaxPointerIntensity());

    this->addNumericParameter(ProcessWidget::tr("Frontal img size")
                              ,std::bind(&VisionSense::AnalogMeterTrackerPC::setFrontalRadius,analogMeterTracker,std::placeholders::_1)
                              ,50
                              ,500
                              ,analogMeterTracker->getFrontalRadius());

    this->addNumericParameter(ProcessWidget::tr("Temporal Filter Window")
                              ,std::bind(&TemporalMeanSensorWrap<TImg,TObj>::setWindSize,filtWrap,std::placeholders::_1)
                              ,1
                              ,50
                              ,filtWrap->getWindSize());

    // Parâmetros do rastreador de posição do manômetro
    this->addSubImageAndRectHandler(ProcessWidget::tr("Track Region"),[=](cv::Mat img,cv::Rect r) {
       posTracker->reset(r);
       posTracker->setTemplate(img);
       analogMeterTracker->setPosition(r);
       frameServerControl->getImgViewer()->redraw();
    });

    this->addBoolParameter(ProcessWidget::tr("Automatic track position"),analogMeterTracker->trackPosEnabled);


    // Define tracker que será executado
    this->setTracker(modbusWrap);
    this->setName(ProcessWidget::tr("Analog Meter Tracker (NonFrontal)"));

}



void AnalogMeterControlNF::drawObject(QPainter *painter, const double &object)
{

//    QGraphicsEllipseItem* ellipse = new QGraphicsEllipseItem();


    if(painter->isActive()){
        painter->setFont(QFont("Times",16));

        QPointF center(analogMeterTracker->getCenterX(),analogMeterTracker->getCenterY());
        // Center the painter at the ellipse origin
        painter->translate(center.x(), center.y());
        // Rotate around its center
        painter->rotate(analogMeterTracker->getTilt());

        // Draw the ellipse
        painter->drawEllipse(QPointF(0,0),analogMeterTracker->getHRadius(),analogMeterTracker->getVRadius());
        painter->drawText(0,0,QString::number(object));

        // Move painter to its initial state
        painter->rotate(-analogMeterTracker->getTilt());
        painter->translate(-center.x(), -center.y());

    }
}
