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
#include "PresenceTrackerControl.h"
#include "ModbusSensorWrap.h"
#include "ModbusConfigWidget.h"

PresenceTrackerControl::PresenceTrackerControl(FrameServerControlWidget* frameServerControl_)
    :TrackerControl(frameServerControl_)
{
    typedef bool TObj;
    typedef cv::Mat TImg;

    //Cria tracker
    presenceTracker = new VisionSense::PresenceTracker();

    //Wrap modbus
    ModbusSensorWrap<TImg,TObj>* modbusWrap = new ModbusSensorWrap<TImg,TObj>(presenceTracker);
    ModbusConfigWidget* modbusWidget = new ModbusConfigWidget();
    modbusWidget->setup(modbusWrap);
    this->getWidget()->addWidget(modbusWidget);

    //Inicia tracker
    this->setTracker(modbusWrap);

    // Adiciona parametros
    this->addMultiClickHandler(ProcessWidget::tr("Track Region"),2,[=] (std::vector<QPoint> points) {
       presenceTracker->topLeft.x= points[0].x();
       presenceTracker->topLeft.y= points[0].y();
       presenceTracker->bottonRight.x= points[1].x();
       presenceTracker->bottonRight.y= points[1].y();

       //if(frameServerControl->getAsyncFrameSrv()->started()==false)
       frameServerControl->getImgViewer()->redraw();
    });
    this->addSubImageHandler(ProcessWidget::tr("Negative reference"),std::bind(&VisionSense::PresenceTracker::setNegativeReference,presenceTracker,std::placeholders::_1));
    this->addSubImageHandler(ProcessWidget::tr("Positive reference"),std::bind(&VisionSense::PresenceTracker::setPositiveReference,presenceTracker,std::placeholders::_1));

    this->setName(ProcessWidget::tr("Presence Tracker"));
}

void PresenceTrackerControl::drawObject(QPainter *painter, const bool &presence)
{
    // Defines a rectangel over area
    const int width = presenceTracker->bottonRight.x-presenceTracker->topLeft.x;
    const int height = presenceTracker->bottonRight.y-presenceTracker->topLeft.y;
    QRect r(presenceTracker->topLeft.x,presenceTracker->topLeft.y,width,height);

    // Draws
    if(presence)
        painter->fillRect(r,this->getDrawColor());
    else
        painter->drawRect(r);
}
