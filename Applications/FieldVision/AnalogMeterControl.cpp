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
#include "AnalogMeterControl.h"
#include "ModbusSensorWrap.h"
#include "ModbusConfigWidget.h"
#include "ProcessWidget.h"

AnalogMeterControl::AnalogMeterControl(FrameServerControlWidget* frameServerControl_)
    :TrackerControl(frameServerControl_)
{
    typedef std::vector<VisionSense::AnalogMeter> TObj;
    typedef cv::Mat TImg;

    //Cria tracker
    VisionSense::AnalogMeterDetectorCLT* analogMeterDetector = new VisionSense::AnalogMeterDetectorCLT();
    VisionCore::Tracker<TImg,TObj>* analogMeterTracker = new VisionCore::DetectorBasedTracker<TImg,VisionSense::AnalogMeter>(analogMeterDetector);

    //Wrap modbus
    ModbusSensorWrap<TImg,TObj>* modbusWrap = new ModbusSensorWrap<TImg,TObj>(analogMeterTracker);
    ModbusConfigWidget* modbusWidget = new ModbusConfigWidget();
    modbusWidget->setup(modbusWrap);
    this->getWidget()->addWidget(modbusWidget);

    //Define tracker que ser� executado
    this->setTracker(modbusWrap);
    this->setName("Analog Meter Detector");

    // Adiciona o controle dos parâmetros do detector
    this->addNumericParameter(ProcessWidget::tr("Pointer proportion"),analogMeterDetector->borderRatio,0.0,1.0,analogMeterDetector->borderRatio);
    this->addNumericParameter(ProcessWidget::tr("Resolution"),analogMeterDetector->resolution,0.0,1.0,analogMeterDetector->resolution);
    this->addNumericParameter(ProcessWidget::tr("Pixel Intensity"),analogMeterDetector->max_pointer_intensity,0,255,analogMeterDetector->max_pointer_intensity);
    this->addBoolParameter(ProcessWidget::tr("Auto detect meters"),analogMeterDetector->autoDetectCircles);

    // Possibilita que o usuário clique no centro e na borda do circulo para definir manualmente a posição do medidor.
    this->addMultiClickHandler(ProcessWidget::tr("Get circle center and border"), 2, [=](std::vector<QPoint> points){
        // Cria circulo baseando-se nos dois pontos clicados

        double radius = sqrt(pow(points[0].x() - points[1].x(),2) + pow(points[0].y() - points[1].y(),2));
        Viscv::Circle<> c(points[0].x(),points[0].y(),radius);

        // Cria lista e seta no detector
        std::vector<Viscv::Circle<>> circles;
        circles.push_back(c);
        analogMeterDetector->setCircles(circles);
    });

}


void AnalogMeterControl::drawObject(QPainter *painter, const std::vector<VisionSense::AnalogMeter> &meterVector)
{
    if(painter->isActive()){
        for(auto meter : meterVector){
            painter->setFont(QFont("Times",16));
            QPoint p1(meter.center.x,meter.center.y);
            QPoint p2(meter.center.x+0.9*meter.radius*sin(meter.pointerAngle * CV_PI / 180.0f) ,
                      meter.center.y+0.9*meter.radius*cos(meter.pointerAngle * CV_PI / 180.0f));
            painter->drawEllipse(p1,(int)meter.radius,(int)meter.radius);
            painter->drawText(p2,QString::number(meter.pointerAngle));
            painter->drawLine(p1,p2);
        }
    }
}
