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

#include "ImageViewerCv.h"
#include "ui_ImageViewerCv.h"
#include <qpainter.h>
#include <qpen.h>

ImageViewerCv::ImageViewerCv(QWidget *parent) :
    QWidget(parent),
    scale(100),
    ui(new Ui::ImageViewerCv)
{
    ui->setupUi(this);
    ui->groupBox->setTitle("WebCam");
}

ImageViewerCv::~ImageViewerCv()
{
    delete ui;
}

void ImageViewerCv::setImage(const cv::Mat img)
{
    cvImg=img;
    //CONVERTE A IMAGEM PARA QIMAGE

    if(cvImg.empty()){
        std::cout<<"Imagem vazia em ImageViewerCv\n";
        std::cout.flush();
        return;
    }

    cv::Mat imgrgb;
    if (img.channels()== 3){
        cv::cvtColor(img, imgrgb, CV_BGR2RGB);
        qtImg = QImage((uchar*)(imgrgb.data),
                     imgrgb.cols,imgrgb.rows,imgrgb.step,QImage::Format_RGB888).copy();
    }
    else
    {
        qtImg = QImage((const unsigned char*)(img.data),
                     img.cols,img.rows,img.step,QImage::Format_Indexed8);
    }
    redraw();
}

void ImageViewerCv::setScale(int scale_)
{
    scale = scale_;
    redraw();
}


void ImageViewerCv::changeTitle(QString tit)
{
    this->ui->groupBox->setTitle(tit);
}

void ImageViewerCv::redraw()
{
    //MOSTRA IMAGEM USANDO PIXMAP
    if(pixmap.isNull())
        pixmap = QPixmap::fromImage(qtImg);
    else
        pixmap.convertFromImage(qtImg);

    QPainter painter(&pixmap);
    emit imageRedraw(&painter);
    const int height = qtImg.height()*scale/100;
    const int width = qtImg.width()*scale/100;
    this->ui->imgDisplay->setFixedSize(width,height);
    this->ui->imgDisplay->setPixmap(pixmap.scaledToHeight(height));
}

void ImageViewerCv::draw(QPainter* painter,const cv::Rect& rect){
    if(painter->isActive()){
        painter->drawRect(rect.x,rect.y,rect.width,rect.height);
    }
}

void ImageViewerCv::draw(QPainter *painter, const std::vector<cv::Rect> &rect)
{
    if(painter->isActive()){
        for(const cv::Rect& r: rect)
            painter->drawRect(r.x,r.y,r.width,r.height);
    }
}

void ImageViewerCv::draw(QPainter* painter,const std::vector<cv::Point2f>& features){
    if(painter->isActive()){
        // Muda grossura da caneta
        QPen paintpen(painter->pen());
        paintpen.setWidth(3);
        painter->setPen(paintpen);
        // Desenha pontos
        for(const cv::Point2f& f : features){
            painter->drawPoint(f.x,f.y);
        }
    }
}

void ImageViewerCv::draw(QPainter* painter,const std::vector<cv::Point>& points){
    if(painter->isActive()){
        // Desenha contorno
        QPolygon polygon;
        for(const cv::Point& p : points){
            polygon.append(QPoint(p.x,p.y));
        }
        painter->drawPolygon(polygon);
    }
}

void ImageViewerCv::draw(QPainter* painter,const cv::Point& point){
    painter->drawPoint(point.x,point.y);
}

void ImageViewerCv::draw(QPainter* painter,const std::vector<std::vector<cv::Point>>& contours){
    if(painter->isActive()){
        // Desenha contornos
        for(const std::vector<cv::Point>& c : contours){
            ImageViewerCv::draw(painter,c);
        }
    }
}

void ImageViewerCv::draw(QPainter* painter,const std::map<long,cv::Rect>& rectList){
    if(painter->isActive()){
        painter->setFont(QFont("Times",20));
        for(const std::pair<long,cv::Rect>& p : rectList){
            const cv::Rect& rect = p.second;
            ImageViewerCv::draw(painter,rect);
            QPoint p1(rect.x,rect.y-5);
            painter->drawText(p1,QString::number(p.first));
        }
    }
}

void ImageViewerCv::draw(QPainter *painter, const std::map<long, std::vector<cv::Point> > &contours)
{
    if(painter->isActive()){
        painter->setFont(QFont("Times",20));
        for(const std::pair<long,std::vector<cv::Point>>& c : contours){
            const std::vector<cv::Point>& contour = c.second;
            ImageViewerCv::draw(painter,contour);
            painter->drawText(contour[0].x,contour[0].y,QString::number(c.first));
        }
    }
}



cv::Mat ImageViewerCv::getCvImg() const
{
    return cvImg;
}


void ImageViewerCv::mouseReleaseEvent(QMouseEvent *mouseEvent)
{

    if (mouseEvent->button() == Qt::LeftButton) {
        const QPoint global = this->mapToGlobal( mouseEvent->pos());
        const QPoint pImg = this->ui->imgDisplay->mapFromGlobal( global );
        emit leftClick(QPoint(pImg.x()*100/scale,pImg.y()*100/scale));
    }
}
