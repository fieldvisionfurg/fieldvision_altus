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

#ifndef ANALOGMETERTRACKERPC_H
#define ANALOGMETERTRACKERPC_H

#include "VisionSensors.h"


namespace VisionSense {

class AnalogMeterTrackerPC : public VisionCore::Tracker<cv::Mat,double>
{
public:
    AnalogMeterTrackerPC();

    // Tracker interface
public:
    const double& getLastTrack();
    void update(const VisionCore::Frame<cv::Mat>& frame);

    int getCenterX() const;
    void setCenterX(int value);

    int getCenterY() const;
    void setCenterY(int value);

    double getVRadius() const;
    void setVRadius(double value);

    double getHRadius() const;
    void setHRadius(double value);

    double getTilt() const;
    void setTilt(double value);

    VisionCore::Tracker<cv::Mat, cv::Rect> *getPositionTracker() const;
    void setPositionTracker(VisionCore::Tracker<cv::Mat, cv::Rect> *value);

    void setPosition(const cv::Rect& pos);

    bool trackPosEnabled;

    int getFrontalRadius() const;
    void setFrontalRadius(int value);

    /// Tamanho proporcional da borda que será ignorada para detectar o ponteiro.
    double getBorderRatio() const;
    void setBorderRatio(double value);

    /// Resolução.
    double getResolution() const;
    void setResolution(double value);

    /// Intensidade do ponteiro.
    int getMaxPointerIntensity() const;
    void setMaxPointerIntensity(int value);

private:
    AnalogMeterDetectorCLT det;
    VisionCore::Tracker<cv::Mat,cv::Rect>* positionTracker;
    void updateHomography();
    int centerX;
    int centerY;
    double vRadius;
    double hRadius;
    double tilt;
    int frontalRadius;
    cv::Mat homography;
    double result;

};

}
#endif // ANALOGMETERTRACKERPC_H
