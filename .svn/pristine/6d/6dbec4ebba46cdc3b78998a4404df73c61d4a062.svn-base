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

#include "VisionSensors.h"

namespace VisionSense {

AnalogMeterTrackerPC::AnalogMeterTrackerPC()
    :centerX(100)
    ,centerY(100)
    ,vRadius(30)
    ,hRadius(20)
    ,tilt(0)
    ,frontalRadius(100)
    ,result(0)
    ,det()
    ,trackPosEnabled(false)
{
    updateHomography();

    det.autoDetectCircles=false;
    std::vector<Viscv::Circle<>> c;
    c.push_back(Viscv::Circle<>(frontalRadius,frontalRadius,frontalRadius));
    det.setCircles(c);
}

void AnalogMeterTrackerPC::updateHomography()
{
    /*
     * syms x y a b cx cy alpha;
    e=(((x-cx)*cos(alpha)-(y-cy)*sin(alpha))/2).^2+(((x-cx)*sin(alpha)+(y-cy)*cos(alpha))/b).^2-1
    r=solve(e == 0,x)
    simplify(r)
    // solve for x
    // (4*cx - 2*y*sin(2*alpha) + 2*b*(2*cy*y - 4*cos(alpha)^2 + b^2*cos(alpha)^2 - cy^2 - y^2 + 4)^(1/2) - 4*cx*cos(alpha)^2 + 2*cy*sin(2*alpha) + b^2*cx*cos(alpha)^2 - (b^2*cy*sin(2*alpha))/2 + (b^2*y*sin(2*alpha))/2)/(b^2*cos(alpha)^2 - 4*cos(alpha)^2 + 4)
    //(4*cx - 2*y*sin(2*alpha) - 2*b*(2*cy*y - 4*cos(alpha)^2 + b^2*cos(alpha)^2 - cy^2 - y^2 + 4)^(1/2) - 4*cx*cos(alpha)^2 + 2*cy*sin(2*alpha) + b^2*cx*cos(alpha)^2 - (b^2*cy*sin(2*alpha))/2 + (b^2*y*sin(2*alpha))/2)/(b^2*cos(alpha)^2 - 4*cos(alpha)^2 + 4)
    solve for y
(b^2*cy - 2*x*sin(2*alpha) + 2*b*(4*cos(alpha)^2 + 2*cx*x - b^2*cos(alpha)^2 + b^2 - cx^2 - x^2)^(1/2) + 4*cy*cos(alpha)^2 + 2*cx*sin(2*alpha) - b^2*cy*cos(alpha)^2 - (b^2*cx*sin(2*alpha))/2 + (b^2*x*sin(2*alpha))/2)/(4*cos(alpha)^2 - b^2*cos(alpha)^2 + b^2)
 -(2*x*sin(2*alpha) - b^2*cy + 2*b*(4*cos(alpha)^2 + 2*cx*x - b^2*cos(alpha)^2 + b^2 - cx^2 - x^2)^(1/2) - 4*cy*cos(alpha)^2 - 2*cx*sin(2*alpha) + b^2*cy*cos(alpha)^2 + (b^2*cx*sin(2*alpha))/2 - (b^2*x*sin(2*alpha))/2)/(4*cos(alpha)^2 - b^2*cos(alpha)^2 + b^2)
 */


    const double t = tilt * M_PI / 180;

    /// Define pontos na elipse
    std::vector<cv::Point2d> srcPoints;
    //top of ellipse
    cv::Point2d p1;
    p1.x=centerX+vRadius*sin(t);
    p1.y=centerY-vRadius*cos(t);

    //botton of ellipse
    cv::Point2d p2;
    p2.x=centerX-vRadius*sin(t);
    p2.y=centerY+vRadius*cos(t);

    //right of ellipse
    cv::Point2d p3;
    p3.x=centerX+hRadius*cos(t);
    p3.y=centerY+hRadius*sin(t);

    //left of ellipse
    cv::Point2d p4;
    p4.x=centerX-hRadius*cos(t);
    p4.y=centerY-hRadius*sin(t);

    srcPoints.push_back(p1);
    srcPoints.push_back(p2);
    srcPoints.push_back(p3);
    srcPoints.push_back(p4);

    /// Define os mesmos pontos na imagem frontal (círculo)
    std::vector<cv::Point2d> dstPoints;
    const float r = static_cast<float>(frontalRadius);
    dstPoints.push_back(cv::Point2d(r,0));
    dstPoints.push_back(cv::Point2d(r,2*r));
    dstPoints.push_back(cv::Point2d(2*r,r));
    dstPoints.push_back(cv::Point2d(0,r));

    /*dstPoints.push_back(cv::Point2f(r+sin(t)*r,sin(t)*r));
    dstPoints.push_back(cv::Point2f(r-sin(t)*r,2*r-sin(t)*r));
    dstPoints.push_back(cv::Point2f(2*r-sin(t)*r,r+sin(t)*r));
    dstPoints.push_back(cv::Point2f(sin(t)*r,r-sin(t)*r));
    */


    /// Calcula matriz de homografia que transforma os pontos para a vista frontal
    homography = cv::findHomography(srcPoints,dstPoints);
}
int AnalogMeterTrackerPC::getFrontalRadius() const
{
    return frontalRadius;
}

void AnalogMeterTrackerPC::setFrontalRadius(int value)
{
    frontalRadius = value;
    updateHomography();
}

double AnalogMeterTrackerPC::getBorderRatio() const
{
    return det.borderRatio;
}

void AnalogMeterTrackerPC::setBorderRatio(double value)
{
    det.borderRatio=value;
}

double AnalogMeterTrackerPC::getResolution() const
{
    return det.resolution;
}

void AnalogMeterTrackerPC::setResolution(double value)
{
    det.resolution = value;
}

int AnalogMeterTrackerPC::getMaxPointerIntensity() const
{
    return det.max_pointer_intensity;
}

void AnalogMeterTrackerPC::setMaxPointerIntensity(int value)
{
    det.max_pointer_intensity=value;
}

double AnalogMeterTrackerPC::getTilt() const
{
    return tilt;
}

void AnalogMeterTrackerPC::setTilt(double value)
{
    tilt = value;
    updateHomography();
}
VisionCore::Tracker<cv::Mat, cv::Rect> *AnalogMeterTrackerPC::getPositionTracker() const
{
    return positionTracker;
}

void AnalogMeterTrackerPC::setPositionTracker(VisionCore::Tracker<cv::Mat, cv::Rect> *value)
{
    positionTracker = value;
}

void AnalogMeterTrackerPC::setPosition(const cv::Rect &pos)
{
    setCenterX(pos.x+pos.width/2);
    setCenterY(pos.y+pos.height/2);
    this->hRadius=pos.width/2;
    this->vRadius=pos.height/2;
    this->tilt=0;
    this->updateHomography();
}


double AnalogMeterTrackerPC::getHRadius() const
{
    return hRadius;
}

void AnalogMeterTrackerPC::setHRadius(double value)
{
    hRadius = value;
    updateHomography();
}

void AnalogMeterTrackerPC::setVRadius(double value)
{
    vRadius = value;
    updateHomography();
}

double AnalogMeterTrackerPC::getVRadius() const
{
    return vRadius;
}

int AnalogMeterTrackerPC::getCenterY() const
{
    return centerY;
}

void AnalogMeterTrackerPC::setCenterY(int value)
{
    centerY = value;
    updateHomography();
}

int AnalogMeterTrackerPC::getCenterX() const
{
    return centerX;
}

void AnalogMeterTrackerPC::setCenterX(int value)
{
    centerX = value;
    updateHomography();
}




const double& AnalogMeterTrackerPC::getLastTrack()
{
    return result;
}

void AnalogMeterTrackerPC::update(const VisionCore::Frame<cv::Mat>& frame)
{
    /// Atualiza a posição
    if(trackPosEnabled && positionTracker!=NULL){
        positionTracker->update(frame);
        if(positionTracker->lostTrack()==false){
          cv::Rect pos = positionTracker->getLastTrack();
          this->setPosition(pos);
        }
    }

    /// Transforma a região do medidor para vista frontal
    cv::Mat frontalImg,frontalImg2;
    const cv::Mat& img=frame.getImg();
    cv::warpPerspective(img,frontalImg,homography,cv::Size(2*frontalRadius+1,2*frontalRadius+1));
    //cv::imshow("Frontal1",frontalImg);
    cv::Mat R = cv::getRotationMatrix2D(cv::Point2d(frontalRadius,frontalRadius), -tilt, 1.0);
    cv::warpAffine(frontalImg, frontalImg2, R, cv::Size(2*frontalRadius+1,2*frontalRadius+1));
    cv::imshow("Frontal",frontalImg2);
    cv::waitKey(1);

    /// Rastreia ponteiro
    std::vector<AnalogMeter> a = det.detect(frontalImg2);
    if(a.size()>0)
        result=a.at(0).pointerAngle;
}

}
