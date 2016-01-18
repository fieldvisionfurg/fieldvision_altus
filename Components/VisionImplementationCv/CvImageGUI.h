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

#ifndef _CVIMAGEGUI_H
#define _CVIMAGEGUI_H

#include "VisionImplementationCv.h"

namespace Viscv
{

/// Esta classe cria uma janela para exibição de imagens.
/** Possui métodos que facilitam a sobreposição de outros
    objetos (pontos, linhas, retangulos, etc) na imagem. 
	
	\ingroup Viscv
*/
class CvImageGUI
{
private:
    const std::string m_windowName;
    cv::Mat m_img;

public:
    /// Constructor method.
    CvImageGUI(cv::Mat img = cv::Mat());
    /// Destructor method.
    virtual ~CvImageGUI();
    void draw(const std::vector<cv::Rect>& rectangles);
    void draw(const std::vector<cv::Point2f>& points);
    void draw(const cv::Rect& rectangle);
    void draw(const std::list<cv::Rect>& rectangles);
    void draw(const cv::Point2f& point);
    void draw(const Circle<>& c);
    void draw(const std::vector<Circle<>>& circles);

	void draw(const std::vector<std::vector<cv::Point>>& contours);

	void draw(const ARTag& tag);
	void draw(const std::vector<ARTag>& tags);

	void draw(const std::map<long,cv::Rect>& objects);


    /// Escreve um texto na imagem.
    void draw(const std::string& text, const double colPosition = 0.5,
              const double rowPosition = 0.5, double size = 1);
    void show();
    /// Returns the value of member 'm_img'.
    const cv::Mat& getImg() const;
    /// Set the value of member 'm_img' to 'img'.
    void setImg(const cv::Mat& img);
    /// Returns the value of member 'm_windowName'.
    const std::string getWindowName() const;
};

}

#endif // ndef _CVIMAGEGUI_H
