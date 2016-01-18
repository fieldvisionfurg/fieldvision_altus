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

// Master include file
#include "VisionImplementationCv.h"

namespace Viscv{

ARTagDetectorBLP::ARTagDetectorBLP()
		: tagHeight(50)
		, tagWidth(50) 
		, borderSize(5)
		, enrolledTags()
		, binarizationThreshold(90)
	{
		// Cadastra algumas tags
        const std::string tagPath="..\\..\\Files\\AugmentedRealityTags\\";
		cv::Mat img;

		img = cv::imread(tagPath+"4x4_384_79.png", CV_LOAD_IMAGE_GRAYSCALE);
		this->enrollTag(img,"tag1");

	}

	ARTagDetectorBLP::~ARTagDetectorBLP(void)
	{
	}

	// Detecta e identifica as tags.
    std::vector<ARTag> ARTagDetectorBLP::detect(const cv::Mat& img) const{
        std::vector<ARTag> tagList;

		cv::Mat gray;
		cv::cvtColor(img, gray, CV_BGR2GRAY);

		// Detecta o contorno externo das tags.
        std::vector<Polygon<int,4>> boxes = detectTagBox(gray);
		// Identifica a tagID de cada uma das tags encontradas.
		for(const Polygon<int,4>& tagBox : boxes){
			// Extrai a imagem interna de uma tag como se fosse vista de frente.
			cv::Mat tagImgCore;
			extractCoreTag(gray,tagBox,tagImgCore);

			// Faz o matching entre a tag encontrada e as tags cadastradas
			std::string tagID="Unknown";
			double minMatch = 10000000000.0;
			const double threshold = 10000000000.0;
			for(const std::pair<cv::Mat,std::string>& enrolledTag : enrolledTags){
				const cv::Mat tagImg = enrolledTag.first;
				double match = matchTag(tagImg,tagImgCore);
				// encontra a tag com menor valor de matching
				if(match<minMatch && match<threshold){
					minMatch=match;
					tagID=enrolledTag.second;
				}
			}
			ARTag tag(tagBox,tagID);
			tagList.push_back(tag);
		}
		return tagList;
	}

	void ARTagDetectorBLP::enrollTag(const cv::Mat& tagImg,const std::string& tagName){
		std::pair<cv::Mat,std::string> tagData(tagImg,tagName);
		enrolledTags.push_back(tagData);
	}


	/// Detecta o contorno externo das tags.
    std::vector<Polygon<int,4>> ARTagDetectorBLP::detectTagBox(const cv::Mat& img) const{
		 // Binariza a imagem
		cv::Mat binary;
		 cv::threshold(img, binary, binarizationThreshold, 255, cv::THRESH_BINARY_INV);

		// Acha contornos dos blobs
		cv::Mat contourOutput = binary.clone();
		std::vector<std::vector<cv::Point> > contours;
		cv::Mat hierarchy;
		cv::findContours( contourOutput, contours, hierarchy,CV_RETR_LIST  , CV_CHAIN_APPROX_SIMPLE );
		
		// Aproxima os contornos externos com um polinômio
		std::vector<std::vector<cv::Point> > contoursPoly( contours.size() );
        for( unsigned i = 0; i < contours.size(); i++ )
		{ 
			approxPolyDP( cv::Mat(contours[i]), contoursPoly[i], 10, true );
		}
		

        std::vector<Polygon<int,4>> poly4;
		// Pega somente os contornos com quatro pontos (retângulos)
		for( auto &c : contoursPoly )
		{   
			if (c.size()==4){

				//Verifica se é quadrado: linhas opostas devem ser paralelas
				//Calcula angulo de cada reta do polygono
				double theta0 = atan2((double)c[1].y-c[0].y,(double)c[1].x-c[0].x);
				double theta1 = atan2((double)c[2].y-c[1].y,(double)c[2].x-c[1].x);
				double theta2 = atan2((double)c[2].y-c[3].y,(double)c[2].x-c[3].x);
				double theta3 = atan2((double)c[3].y-c[0].y,(double)c[3].x-c[0].x);

				double t=0.1; //tolerancia para igualdade em radianos

				if(abs(theta0-theta2)<t && abs(theta1-theta3)<t){
					Polygon<> p;
					p.vertices[0]=c[0];
					p.vertices[1]=c[1];
					p.vertices[2]=c[2];
					p.vertices[3]=c[3];
					poly4.push_back(p);
				}
			}
		}
		
		return poly4;
    }

	/// Extrai a imagem interna de uma tag como se fosse vista de frente. Utiliza transformação de perspectiva.
	void ARTagDetectorBLP::extractCoreTag(const cv::Mat& img,const Polygon<int,4>& tagBox, cv::Mat& tagImgOut) const{
		// Inicialmente, calcula a matriz de homografia. Então transforma a região da tag para vista frontal e retira as bordas de contorno,
		// resultando em uma imagem frontal do núcleo da tag.

		// converte pontos do contorno da tag para formato da opencv
		std::vector<cv::Point2f> srcPoints;
		for(int i=0;i<4;i++){
            cv::Point2f p(static_cast<float>(tagBox.vertices[i].x),
                          static_cast<float>(tagBox.vertices[i].x));
			srcPoints.push_back(p);
		}

		// define os limites padrões da tag (vista frontal com tamanho padrão)
		std::vector<cv::Point2f> dstPoints;
		dstPoints.push_back(cv::Point2f(0,0));
        dstPoints.push_back(cv::Point2f(0,static_cast<float>(tagWidth-1)));
        dstPoints.push_back(cv::Point2f(static_cast<float>(tagHeight-1),static_cast<float>(tagWidth-1)));
        dstPoints.push_back(cv::Point2f(static_cast<float>(tagHeight-1),0));

		// calcula matriz de homografia que converte os pontos da tag para a vista frontal
		cv::Mat H = cv::findHomography(srcPoints,dstPoints);

		// transforma região da tag utilizando a matrix de homografia
		cv::Mat tagImg;
		cv::warpPerspective(img,tagImg,H,cv::Size(tagWidth,tagHeight));

		// remove bordas externas da tag
		cv::Rect c(borderSize,borderSize,tagWidth-2*borderSize,tagHeight-2*borderSize);
		tagImgOut=tagImg(c);
	}

	/// Faz o matching entre uma imagem e uma tag, ambas frontais, retornando um matching score.
	double ARTagDetectorBLP::matchTag(const cv::Mat& img,const cv::Mat& tagCoreImg) const {
		cv::Mat imgResized;
		cv::resize(img,imgResized,cv::Size(this->tagWidth-2*this->borderSize,this->tagHeight-2*this->borderSize));
		double distance = cv::norm(imgResized,tagCoreImg,cv::NORM_L2);
		return distance;
	}
}
