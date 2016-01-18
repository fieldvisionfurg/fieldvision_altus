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

#pragma once

#include "VisionSensors.h"

namespace VisionSense {

/// Rastreador de nível de um líquido ou sólido dentro de um container.
/** Um container é definido na imagem por um retângulo através de um ponto esquerdo-superior e outro ponto 
 * direito inferior.
 *
 * A inferência do nível de preenchimento do container é feita através da análise da cor de cada
 * linha ao longo do container; para cada linha, é calculado a cor média da linha e comparado com as cores positivas
 * (presença de líquido) e negativa (ausência de líquido).
 *
 * \ingroup VisionSensors
*/
class DiffractionLevelTracker :
    public VisionCore::Tracker<cv::Mat,double>
{
public:
	DiffractionLevelTracker(void);
	virtual ~DiffractionLevelTracker(void);

    void update(const VisionCore::Frame<cv::Mat>& frame);

	const double& getLastTrack();

	bool isHorizontal;

	cv::Point topLeft;
	cv::Point bottonRight;
    bool trackPosEnabled;

    VisionCore::Tracker<cv::Mat, cv::Rect> *getPositionTracker() const;
    void setPositionTracker(VisionCore::Tracker<cv::Mat, cv::Rect> *value);

    bool getTrackingPosition() const;
    void setTrackingPosition(bool value);
    void setPosition(const cv::Rect& pos);

private:
    VisionCore::Tracker<cv::Mat,cv::Rect>* positionTracker;
    double level;

};

}
