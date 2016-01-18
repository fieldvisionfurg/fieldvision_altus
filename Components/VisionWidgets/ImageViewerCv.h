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

#ifndef IMAGEVIEWERCV_H
#define IMAGEVIEWERCV_H

#include <QWidget>
#include <qevent.h>
#include "VisionCore.h"
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"

namespace Ui {
class ImageViewerCv;
}

/// Esta classe implementa um widget capaz de mostrar uma imagem do tipo cv::Mat.
/**
 *
 * \ingroup VisionGUI
 */
class ImageViewerCv : public QWidget
{
    Q_OBJECT

public:
    explicit ImageViewerCv(QWidget *parent = 0);
    ~ImageViewerCv();

    void changeTitle(QString tit);

    void redraw();

    /// Desenha um retângulo.
    static void draw(QPainter* painter,const cv::Rect& rect);

    static void draw(QPainter* painter,const std::vector<cv::Rect>& rect);

    static void draw(QPainter *painter, const std::vector<cv::Point2f>& features);

    static void draw(QPainter* painter,const std::vector<cv::Point>& points);

    static void draw(QPainter *painter, const std::vector<std::vector<cv::Point>>& contours);

    static void draw(QPainter *painter, const cv::Point& point);

    static void draw(QPainter *painter, const std::map<long,cv::Rect>& rectList);

    static void draw(QPainter *painter, const std::map<long,std::vector<cv::Point>>& contours);

    template<typename T>
    static void draw(QPainter *painter, const T& obj)
    {
        //default: do nothing
    }

    cv::Mat getCvImg() const;

public slots:
    void setImage(const cv::Mat img);

    void setScale(int scale_);

signals:
    void imageRedraw(QPainter* painter);
    void leftClick(QPoint point);

private:
    Ui::ImageViewerCv *ui;
    QImage qtImg;
    cv::Mat cvImg;
    QPixmap  pixmap;
    int scale;

    void mouseReleaseEvent(QMouseEvent* mouseEvent);
};

#endif // IMAGEVIEWERCV_H
