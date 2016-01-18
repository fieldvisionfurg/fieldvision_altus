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


LevelTracker::LevelTracker(void)
    : regOk(false)
    , negColOk(false)
    , posColOk(false)
{
}

LevelTracker::~LevelTracker(void)
{
}

void LevelTracker::update(const VisionCore::Frame<cv::Mat>& frame){
	// Cria uma sub-imagem contendo a "region of interest"
	cv::Rect roi(topLeft,bottonRight);
	const cv::Mat& originalImg = frame.getImg();
	cv::Mat img = originalImg(roi);

	// Converte para espaço de cor Lab
	cv::Mat imgLab;
	cv::cvtColor(img,imgLab,CV_BGR2Lab);

	// Para cada linha, calcula distâncias para referências
	std::vector<double> distPos(img.rows);  //distância cumulativa para a referência em cada linha
	std::vector<double> distNeg(img.rows);
	double distPosCum=0;
	double distNegCum=0;
	for(int i=0;i<imgLab.rows;i++){
		cv::Mat row=imgLab.row(i);
		cv::Scalar meanColor = cv::mean(row);
		distPos[i]=distPosCum+cv::norm(meanColor,positiveColorLab,cv::NORM_L2);
		distNeg[i]=distNegCum+cv::norm(meanColor,negativeColorLab,cv::NORM_L2);
		distPosCum=distPos[i];
		distNegCum=distNeg[i];
	}

	// Decide nível. Calcula o erro de decisão para cada linha.
	double minError=std::numeric_limits<double>::max();
	int minErrorIndex=0;
	for(int i=0;i<imgLab.rows;i++){
		double error=distNeg[i]+distPos[imgLab.rows-1]-distPos[i];
		if(error<minError){
			minError=error;
			minErrorIndex=i;
		}
	}
	level=(imgLab.rows-minErrorIndex)/(double)imgLab.rows;

    /// True se algo não está configurado
    m_lostTrack = !negColOk || !posColOk || !regOk;
}

const double& LevelTracker::getLastTrack() {
	return level;

}

bool LevelTracker::lostTrack() const
{
    return m_lostTrack;
}

/// Set the negative presence (lack of object) based on a reference image.
void LevelTracker::setNegativeReference(const cv::Mat& img){
	// Calcula intensidade média do pixel no espaço de cor Lab
    if(!img.empty()){
        cv::Mat imgLab;
        cv::cvtColor(img,imgLab,CV_BGR2Lab);
        negativeColorLab = cv::mean(imgLab);
        negColOk=true;
    }
}

/// Set the positive presence (object is present) based on a reference image.
void LevelTracker::setPositiveReference(const cv::Mat& img){
	// Calcula intensidade média do pixel no espaço de cor Lab
    if(!img.empty()){
        cv::Mat imgLab;
        cv::cvtColor(img,imgLab,CV_BGR2Lab);
        positiveColorLab = cv::mean(imgLab);
        posColOk=true;
    }
}

void LevelTracker::setRegion(const cv::Rect &roi)
{
    topLeft=roi.tl();
    bottonRight=roi.br();
    if(topLeft.x<bottonRight.x && topLeft.y<bottonRight.y)
        regOk=true;
}

}
