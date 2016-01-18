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

#ifndef _CIRCLEDETECTORHTCF_H
#define _CIRCLEDETECTORHTCF_H

#include "VisionImplementationCv.h"

namespace Viscv
{

/// Defines a circle. Stores the center (x,y) and radius.
template<typename T = int>
class Circle
{
public:
    T m_radius;
    T m_x;
    T m_y;
    /// Constructor method.
    Circle(T x, T y, T radius)
		: m_radius(radius)
		, m_x(x)
		, m_y(y)
	{ 	}
    /// Destructor method.
	virtual ~Circle() { }
};


/// Detects circles using Hough Transform and (optionally) Color Filter (HTCF).
/**
\ingroup Viscv
*/
class CircleDetectorHTCF
    : public VisionCore::Detector<cv::Mat,Circle<>>
{
public:
    /// Constructor method.
    CircleDetectorHTCF();
    /// Destructor method.
    virtual ~CircleDetectorHTCF();
    /// 
    std::vector<Circle<>> detect(const cv::Mat& img) const;

	/// Computa uma imagem contendo somente os pixels com hue entre os limites especificados em meanHue e varHue.
    void colorFilter(const cv::Mat& src, cv::Mat& dst) const;

	/// Desvio padrão do filtro Gaussiano (relativo ao número de linhas da imagem)
	/** O valor real do desvio padrão utilizado é img.rows * gaussFilterSigma*/
	double gaussFilterSigma;

	/// Parametro 1 da transformada de Hough na OpenCv
	double houghParam1;

	/// Parametro 2 da transformada de Hough na OpenCv
	double houghParam2;

	/// Hue médio do filtro de cor [0,179].
    int meanHue; 

    /// Variância (tolerância) do filtro de cor [0,90].
	/** O filtro de cor retem apenas pixels com hue entre [meanHue-varHue , meanHue+varHue]*/
    int varHue;

	/// Define se o filtro de cor deve ser utilizado ou não.
	bool useColorFilter;

	/// Define se o filtro gaussiano (blur) deve ser utilizado ou não.
	bool useBlur;

	/// Define se as imagens intermediárias de processamento devem ser mostradas.
	bool showProcessingImgs;
};
}

#endif // ndef _CIRCLEDETECTORHTCF_H
