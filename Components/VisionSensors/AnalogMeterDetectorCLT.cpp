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

AnalogMeterDetectorCLT::AnalogMeterDetectorCLT(void)
    : borderRatio(0.8)
	, resolution(.1f)
	, max_pointer_intensity(50)
	, circleDet(  )
	, autoDetectCircles(true)
    , circles( )
{

}


AnalogMeterDetectorCLT::~AnalogMeterDetectorCLT(void)
{
}

// Função para achar o ângulo entre a reta e uma reta horizontal em x
void AnalogMeterDetectorCLT::getAngleBetween(cv::Point p1, cv::Point p2, double &angle) const {
	angle = (atan2(p2.y - p1.y, p2.x - p1.x) * 180.0f / CV_PI) /*- 90.0f*/; // Incluído - Retirado cvRound

	if (angle < .0f){
		angle = angle + 360.0f;
	}
}

std::vector<AnalogMeter> AnalogMeterDetectorCLT::detect(const cv::Mat& img) const
{

    std::vector<AnalogMeter> meters;
    cv::Mat gray,blurred;
    /// Convertendo a imagem para tons de cinza
    if(img.channels()>1)
        cvtColor(img, gray, CV_BGR2GRAY);
    else
        gray=img;
    if(autoDetectCircles)
        circles = circleDet.detect(gray);


    /// Processa cada circulo encontrado
    for(Viscv::Circle<>& circle: circles){
        cv::Point topLeft(circle.m_x - circle.m_radius,circle.m_y - circle.m_radius);
        cv::Point bottonRight(circle.m_x + circle.m_radius,circle.m_y + circle.m_radius);
        //Verifica se o circulo está todo dentro da imagem
        if(topLeft.x>=0 && topLeft.y>=0 && bottonRight.x<=img.cols && bottonRight.y<=img.rows){
            //Recortando apenas um quadrado que englobe o manômetro
            cv::Rect ROI(topLeft,bottonRight);
            cv::Mat recorte = gray(ROI);

            // Para cada angulo, calcula a soma dos pixels que passam por uma reta com origem no centro
            //Considera apenas pontos dentro do círculo (não os do quadrado), achando a hipotenusa e comparado ao raio e para cada raio calcula a intensidade
            int radius = circle.m_radius;
            cv::Point new_center(radius, radius);
            double radius2 = pow(borderRatio*radius, 2); // reduz o raio da área a ser processado para não pegar a borda do medidor
            double angulo;
            int positions = int(360.0 / resolution);
            std::vector<int> intensity(positions,0);
            std::vector<int> count(intensity.size(),0);

            // Computa um "histograma" da intesidade dos pixels, onde as bins são os angulos
            for (int i = 0; i < recorte.cols; i++){
                for (int j = 0; j < recorte.rows; j++){
                    getAngleBetween(new_center, cv::Point(i, j), angulo);
                    int indice = (int)(angulo /resolution);
                    if (pow(new_center.x - i, 2) + pow(new_center.y - j, 2) <= radius2)
                    {
                        int pixelIntensity = recorte.at<unsigned char>(i, j);
                        intensity[indice] = intensity[indice] + (255-pixelIntensity);
                        count[indice]++;
                    }
                }
            }

            // Pega o angulo com maior intesidade média
            int maxValue=-1;
            int maxIndex=-1;
            //cv::blur(intensity, intensity, cv::Size(1, 5));
            for(unsigned int i=0;i<intensity.size()-1;i++){
                if(count[i]>5){
                    if(intensity[i]/count[i]>maxValue){
                        maxValue=intensity[i]/count[i];
                        maxIndex=i;
                    }
                }
            }

            //Cria a estrutura com os dados do medidor
            AnalogMeter m;
            m.center=cv::Point(circle.m_x,circle.m_y);
            m.radius=radius;
            m.pointerAngle=maxIndex*resolution;
            meters.push_back(m);
        }
    }
    return meters;
}

void AnalogMeterDetectorCLT::setCircles(const std::vector<Viscv::Circle<>>& c){
	circles=c;
	autoDetectCircles=false;
}

}
