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


DiffractionLevelTracker::DiffractionLevelTracker(void)
    :positionTracker(NULL)
    ,trackPosEnabled(false)
{
}

DiffractionLevelTracker::~DiffractionLevelTracker(void)
{
}

void DiffractionLevelTracker::update(const VisionCore::Frame<cv::Mat>& frame)
{
    // Update position
    if(trackPosEnabled && positionTracker!=NULL){
        positionTracker->update(frame);
        if(positionTracker->lostTrack()==false){
          cv::Rect pos = positionTracker->getLastTrack();
          this->setPosition(pos);
        }
    }


	// Cria uma sub-imagem contendo a "region of interest"
    cv::Rect roi(topLeft, bottonRight);
	const cv::Mat& originalImg = frame.getImg();
	cv::Mat img = originalImg(roi);

	// Converte para espaço de cor Lab

    cv::Mat imgGray;
    if(img.channels()>1){
        cv::cvtColor(img,imgGray,CV_BGR2GRAY);
    }else{
        imgGray = img;
    }

    //aplica filtro gradiente
    cv::Mat gradient;
    cv::Sobel(imgGray, gradient, CV_16S, 0 , 1);

    // Busca a linha que apresenta o maior gradiente
    double maxGradient = std::numeric_limits<double>::min();
    int maxGradientIndex = 0;
    for(int i=0;i<gradient.rows;i++){

        cv::Scalar gradienteLinha = cv::sum(gradient.row(i));
        if(gradienteLinha[0]> maxGradient){
            maxGradient=gradienteLinha[0];
            maxGradientIndex=i;
		}
	}

    //Pela refração, as duas partes precisam ter médias diferentes. O liquido é afetado pela luz.
    double threshold = 5;
    if(abs(cv::mean(imgGray(cv::Rect(0,0,imgGray.cols, maxGradientIndex)))[0] - cv::mean(imgGray(cv::Rect(0,maxGradientIndex,imgGray.cols, imgGray.rows - maxGradientIndex)))[0]) > threshold){
        level=(imgGray.rows-maxGradientIndex)/(double)imgGray.rows;
    }else{
        level = 0;
    }
}

const double& DiffractionLevelTracker::getLastTrack()
{
	return level;
}
VisionCore::Tracker<cv::Mat, cv::Rect> *DiffractionLevelTracker::getPositionTracker() const
{
    return positionTracker;
}

void DiffractionLevelTracker::setPositionTracker(VisionCore::Tracker<cv::Mat, cv::Rect> *value)
{
    positionTracker = value;
}
bool DiffractionLevelTracker::getTrackingPosition() const
{
    return trackPosEnabled;
}

void DiffractionLevelTracker::setTrackingPosition(bool value)
{
    trackPosEnabled = value;
}

void DiffractionLevelTracker::setPosition(const cv::Rect &pos)
{
    this->topLeft.x=pos.x;
    this->topLeft.y=pos.y;
    this->bottonRight.x=pos.x+pos.width;
    this->bottonRight.y=pos.y+pos.height;
}

}//namespace VisionSense

