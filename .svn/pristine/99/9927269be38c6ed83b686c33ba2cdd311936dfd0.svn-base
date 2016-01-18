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

#include "FrameServerControlWidget.h"
#include "ui_FrameServerControlWidget.h"


FrameServerControlWidget::FrameServerControlWidget(VisionCore::FrameServer<cv::Mat> *frameServer_,
                                                   bool startPaused,
                                                   int framesPerSecond ,
                                                   QWidget *parent)
    :QWidget(parent)
    ,ui(new Ui::FrameServerControlWidget)
    ,frameServer(frameServer_)
    ,asyncFrameSrv(new VisionCore::AsyncFrameServerWrap<cv::Mat>(frameServer_,framesPerSecond))
    ,imgViewer(new ImageViewerCv())
    ,previousFrame(NULL)
{
    qRegisterMetaType<cv::Mat>("cv::Mat");

    ui->setupUi(this);
    ui->startBtn->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
    ui->startBtn->setText("");
    ui->stopBtn->setIcon(style()->standardIcon(QStyle::SP_MediaStop));
    ui->stopBtn->setText("");
    ui->closeBtn->setIcon(style()->standardIcon(QStyle::SP_TitleBarCloseButton));
    ui->closeBtn->setText("");

    //Quando o frame server captura uma imagem, chama esta função callback, que mostra a imagem no image viewer
    asyncFrameSrv->setCallback([=](std::shared_ptr<VisionCore::Frame<cv::Mat>> frame){
        // Delays display one frame so processing and view is in the same pace
        if(previousFrame)
            emit newImage(previousFrame->getImg());
        previousFrame=frame;
    });
    QObject::connect(this,&FrameServerControlWidget::newImage,imgViewer,&ImageViewerCv::setImage);

    //Sends change in image scale to ImageViewer.
    QObject::connect(ui->scaleSlider,&QSlider::valueChanged,imgViewer,&ImageViewerCv::setScale);

    if(startPaused){
        // Gets only the first two frame so display can start
        asyncFrameSrv->setFramesToCapture(2);
    }
    asyncFrameSrv->start();
}

FrameServerControlWidget::~FrameServerControlWidget()
{
    //asyncFrameSrv->stop();
    delete ui;
    delete asyncFrameSrv;
    delete frameServer;
    delete imgViewer;
}
VisionCore::AsyncFrameServerWrap<cv::Mat> *FrameServerControlWidget::getAsyncFrameSrv() const
{
    return asyncFrameSrv;
}
ImageViewerCv *FrameServerControlWidget::getImgViewer() const
{
    return imgViewer;
}
QString FrameServerControlWidget::getName() const
{
    return name;
}

void FrameServerControlWidget::setName(const QString &value)
{
    name = value;
    ui->groupBox->setTitle(name);
    imgViewer->changeTitle(name);
}


void FrameServerControlWidget::on_startBtn_clicked()
{
    asyncFrameSrv->start();
}

void FrameServerControlWidget::on_stopBtn_clicked()
{
    asyncFrameSrv->stop();
}

void FrameServerControlWidget::on_closeBtn_clicked()
{
    this->~FrameServerControlWidget();
}

void FrameServerControlWidget::on_fpsInput_valueChanged(double arg1)
{
    this->asyncFrameSrv->setFramesPerSecond(arg1);
}
