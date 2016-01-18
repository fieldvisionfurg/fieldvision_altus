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

#ifndef FRAMESERVERCONTROLWIDGET_H
#define FRAMESERVERCONTROLWIDGET_H

#include <QWidget>
#include "VisionCore.h"
#include "ImageViewerCv.h"

Q_DECLARE_METATYPE(cv::Mat)

namespace Ui {
class FrameServerControlWidget;
}

/// Esta classe controla a execução e representação gráfica de um vídeo (FrameServer).
/**
 *
 * \ingroup VisionGUI
 */
class FrameServerControlWidget : public QWidget
{
    Q_OBJECT

public:
    explicit FrameServerControlWidget(VisionCore::FrameServer<cv::Mat>* frameServer_,
                                      bool startPaused = false ,
                                      int framesPerSecond = 25 ,
                                      QWidget *parent = 0);
    virtual ~FrameServerControlWidget();

    VisionCore::AsyncFrameServerWrap<cv::Mat> *getAsyncFrameSrv() const;

    ImageViewerCv *getImgViewer() const;

    QString getName() const;
    void setName(const QString &value);

signals:
    void newImage(cv::Mat img);


private slots:
    void on_startBtn_clicked();

    void on_stopBtn_clicked();

    void on_closeBtn_clicked();

    void on_fpsInput_valueChanged(double arg1);

protected:
    Ui::FrameServerControlWidget *ui;
    VisionCore::FrameServer<cv::Mat>* frameServer;
    VisionCore::AsyncFrameServerWrap<cv::Mat>* asyncFrameSrv;
    ImageViewerCv* imgViewer;
    QString name;
    std::shared_ptr<VisionCore::Frame<cv::Mat>> previousFrame;

};


#endif // FRAMESERVERCONTROLWIDGET_H
