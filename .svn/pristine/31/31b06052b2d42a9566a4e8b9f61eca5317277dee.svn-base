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

namespace Viscv
{

TemplMatchingDetector::TemplMatchingDetector()
    : matchMethod(CV_TM_SQDIFF_NORMED)
    , m_ready(false)
{

}

TemplMatchingDetector::~TemplMatchingDetector()
{

}

std::vector<cv::Rect> TemplMatchingDetector::detect(const cv::Mat &img) const
{
    std::vector<cv::Rect> result;
    if(m_ready && img.rows>=templImg.rows && img.cols>=templImg.cols){
        cv::Mat match,gray;

        /// Convertendo a imagem para tons de cinza
        if(img.channels()>1)
            cvtColor(img, gray, CV_BGR2GRAY);
        else
            gray=img;

        /// Match
        cv::matchTemplate(gray,templImg,match,matchMethod);

        /// Localizing the best match with minMaxLoc
        double minVal; double maxVal; cv::Point minLoc; cv::Point maxLoc;
        cv::Point matchLoc;
        cv::minMaxLoc( match, &minVal, &maxVal, &minLoc, &maxLoc, cv::Mat() );

        /// For SQDIFF and SQDIFF_NORMED, the best matches are lower values. For all the other methods, the higher the better
        if( matchMethod  == CV_TM_SQDIFF || matchMethod == CV_TM_SQDIFF_NORMED )
            matchLoc = minLoc;
        else
            matchLoc = maxLoc;

        cv::Rect r(matchLoc.x,matchLoc.y,templImg.cols,templImg.rows);

        //TODO: implementar retorno de multiplos objetos
        result.push_back(r);
    }
    return result;

}
cv::Mat TemplMatchingDetector::getTemplate() const
{
    return templImg;
}

void TemplMatchingDetector::setTemplate(const cv::Mat &templ)
{
    /// Convertendo a imagem para tons de cinza
    if(templ.channels()>1)
        cvtColor(templ, templImg, CV_BGR2GRAY);
    else
        templImg=templ;
    m_ready=true;
}
int TemplMatchingDetector::getMatchMethod() const
{
    return matchMethod;
}

void TemplMatchingDetector::setMatchMethod(int value)
{
    matchMethod = value;
}
bool TemplMatchingDetector::ready() const
{
    return m_ready;
}

}
