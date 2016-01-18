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

#ifndef ANALOGMETERDETECTORCLT_H
#define ANALOGMETERDETECTORCLT_H

#include "VisionSensors.h"

namespace VisionSense {

/// Estrutura para armazenar a posição e leitura de um medidor analógico.
/**
 * \ingroup VisionSensors
*/
struct AnalogMeter {
	double measure;
	double pointerAngle;
	cv::Point center;
    double radius;
};


/// Detector de medidores analógicos.
/** Assume-se que o medidor é circular, com fundo branco e ponteiro preto.
 *
 *\ingroup VisionSensors
*/
class AnalogMeterDetectorCLT :
    public VisionCore::Detector<cv::Mat,AnalogMeter>
{
public:
	/// Construtor.
	AnalogMeterDetectorCLT(void);

	/// Destrutor.
	~AnalogMeterDetectorCLT(void);

    std::vector<AnalogMeter> detect(const cv::Mat& img) const;

	/// Detector de circulos para detectar contorno do manometro.
    Viscv::CircleDetectorHTCF circleDet;

	/// Tamanho proporcional da borda que será ignorada para detectar o ponteiro.
	double borderRatio;

	/// Resolução.
	double resolution;

	/// Intensidade do ponteiro.
	int max_pointer_intensity;

	/// Detectar automaticamente os circulos dos medidores?
	bool autoDetectCircles;

	/// Permite definir manualmente os circulos a serem processados (desabilita detecção automática)
    void setCircles(const std::vector<Viscv::Circle<>>& c);



private:
	/// Função para achar o ângulo entre a reta e uma reta horizontal em x
	void getAngleBetween(cv::Point p1, cv::Point p2, double &angle) const;

	/// Lista com os circulos a serem processados.
    mutable std::vector<Viscv::Circle<>> circles;

};

}
#endif // ANALOGMETERDETECTORCLT_H
