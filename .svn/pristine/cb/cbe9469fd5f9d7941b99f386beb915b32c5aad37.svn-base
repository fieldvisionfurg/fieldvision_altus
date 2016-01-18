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
#include "LevelTrackerControl.h"
#include "ModbusSensorWrap.h"
#include "ModbusConfigWidget.h"
#include "FieldVisionCore.h"

LevelTrackerControl::LevelTrackerControl(FrameServerControlWidget* frameServerControl_)
    :TrackerControl(frameServerControl_)
{
    typedef double TObj;
    typedef cv::Mat TImg;

    //Cria tracker
    levelTracker = new VisionSense::LevelTracker();

    //Cria filtro de média temporal
    /*
    TemporalMeanSensorWrap<TImg,TObj>* filtWrap = new TemporalMeanSensorWrap<TImg,TObj>(levelTracker);
    this->addNumericParameter("Temporal Filter",
                              std::bind(&TemporalMeanSensorWrap<TImg,TObj>::setWindSize,filtWrap,std::placeholders::_1),
                              1,50,filtWrap->getWindSize());
    */

    //Cria filtro de partículas
    PartFiltSensorDoubleWrap<TImg>* filtWrap = new PartFiltSensorDoubleWrap<TImg>(levelTracker);
    filtWrap->setStdDeviation(0.1);
    this->addNumericParameter("Part. Filt. Std",
                              std::bind(&PartFiltSensorDoubleWrap<TImg>::setStdDeviation,filtWrap,std::placeholders::_1),
                              0.01,1.0,filtWrap->getStdDeviation());

    //Wrap modbus
    ModbusSensorWrap<TImg,TObj>* modbusWrap = new ModbusSensorWrap<TImg,TObj>(filtWrap);
    ModbusConfigWidget* modbusWidget = new ModbusConfigWidget();
    modbusWidget->setup(modbusWrap);
    this->getWidget()->addWidget(modbusWidget);

    // Adiciona parametros
    this->addMultiClickHandler("Track Region",2,[=] (std::vector<QPoint> points) {
       levelTracker->setRegion(cv::Rect(points[0].x(),points[0].y(),points[1].x()-points[0].x(),points[1].y()-points[0].y()));
       frameServerControl->getImgViewer()->redraw();
    });
    this->addSubImageHandler("Negative reference",std::bind(&VisionSense::LevelTracker::setNegativeReference,levelTracker,std::placeholders::_1));
    this->addSubImageHandler("Positive reference",std::bind(&VisionSense::LevelTracker::setPositiveReference,levelTracker,std::placeholders::_1));

    //Inicia tracker
    this->setTracker(modbusWrap);
    this->setName("Level Tracker");
}

void LevelTrackerControl::drawObject(QPainter *painter, const double &level)
{
    // Desenha retangulo sobre are observada
    const int width = levelTracker->bottonRight.x-levelTracker->topLeft.x;
    const int height = levelTracker->bottonRight.y-levelTracker->topLeft.y;
    painter->drawRect(levelTracker->topLeft.x,levelTracker->topLeft.y,width,height);

    // Draw line at measured level
    QPoint left(levelTracker->topLeft.x, levelTracker->bottonRight.y-level*height);
    QPoint right(levelTracker->bottonRight.x, levelTracker->bottonRight.y-level*height);
    painter->drawLine(left,right);

    // Write level
    painter->setFont(QFont("Times",15));
    QPoint textP(levelTracker->topLeft.x-25,levelTracker->bottonRight.y-level*height);
    painter->drawText(textP,QString::number(int(level*100)));
}

