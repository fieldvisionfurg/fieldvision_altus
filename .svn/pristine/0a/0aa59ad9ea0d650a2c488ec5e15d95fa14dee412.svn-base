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


/*
Constructor method.
*/
CvImageGUI::CvImageGUI(cv::Mat img)
    : m_windowName("Vision Framework")
    , m_img(img)
{
	cv::namedWindow(m_windowName,CV_WINDOW_AUTOSIZE);

}


/*
Destructor method.
*/
CvImageGUI::~CvImageGUI()
{
    // close the window
     cv::destroyWindow(m_windowName);

}


void CvImageGUI::draw(const std::vector<cv::Rect>& rectangles)
{
    for(unsigned int i=0;i<rectangles.size();i++){
        this->draw(rectangles.at(i));
    }
    
}


void CvImageGUI::draw(const std::vector<cv::Point2f>& points)
{
    for(unsigned int i=0;i<points.size();i++){
        this->draw(points.at(i));
    }
}


void CvImageGUI::draw(const cv::Rect& rectangle)
{
    cv::rectangle(m_img,rectangle.tl(),rectangle.br(),CV_RGB(0,0,255));
}


void CvImageGUI::draw(const std::list<cv::Rect>& rectangles)
{
    for(const cv::Rect& r : rectangles){
        this->draw(r);
    }
}

void CvImageGUI::draw(const ARTag& tag){
	cv::line(m_img,cv::Point(tag.box.vertices[0].x,tag.box.vertices[0].y),cv::Point(tag.box.vertices[1].x,tag.box.vertices[1].y),CV_RGB(255,0,0));
	cv::line(m_img,cv::Point(tag.box.vertices[1].x,tag.box.vertices[1].y),cv::Point(tag.box.vertices[2].x,tag.box.vertices[2].y),CV_RGB(255,0,0));
	cv::line(m_img,cv::Point(tag.box.vertices[2].x,tag.box.vertices[2].y),cv::Point(tag.box.vertices[3].x,tag.box.vertices[3].y),CV_RGB(255,0,0));
	cv::line(m_img,cv::Point(tag.box.vertices[3].x,tag.box.vertices[3].y),cv::Point(tag.box.vertices[0].x,tag.box.vertices[0].y),CV_RGB(255,0,0));
	double xRatio = ((double)tag.box.vertices[0].x)/m_img.cols;
	double yRatio = ((double)tag.box.vertices[0].y)/m_img.rows;
	this->draw(tag.tagID,xRatio,yRatio);
}

void CvImageGUI::draw(const std::vector<ARTag>& tags){
	for(const ARTag& t : tags)
		draw(t);
}

void CvImageGUI::draw(const std::vector<std::vector<cv::Point>>& contours){
	cv::drawContours(m_img,contours,-1,CV_RGB(255,0,0));
}


void CvImageGUI::draw(const cv::Point2f& point)
{
    //circle(Mat& img, Point center, int radius, const Scalar& color, int thickness=1, int lineType=8, int shift=0)
    circle(m_img, point, 3 , CV_RGB(0,0,255));
}


void CvImageGUI::draw(const Circle<>& c)
{
    circle(m_img, cv::Point((int)c.m_x,(int)c.m_y), (int)c.m_radius , CV_RGB(0,0,255));
}


void CvImageGUI::draw(const std::vector<Circle<>>& circles)
{
    for(const Circle<>& c : circles)
        draw(c);
}

void CvImageGUI::draw(const std::map<long,cv::Rect>& objects)
{
	for(const std::pair<long,cv::Rect>& obj : objects){
		const cv::Rect& rect = obj.second;
		draw(rect);
		std::stringstream ss;
		ss << obj.first;
		cv::putText(m_img,ss.str(),cv::Point(rect.x,rect.y),cv::FONT_HERSHEY_SIMPLEX,1,CV_RGB(0,0,255));
	}
}

/*
Escreve um texto na imagem.
*/
void CvImageGUI::draw(const std::string& text, const double colPosition,
                             const double rowPosition, double size)
{
    cv::putText(m_img,text,cv::Point(static_cast<int>(colPosition*m_img.cols),static_cast<int>(rowPosition*m_img.rows)),cv::FONT_HERSHEY_SIMPLEX,size,CV_RGB(0,0,255));
}

void CvImageGUI::show()
{
	if(!m_img.empty()){
		cv::imshow(m_windowName,m_img);
		cv::waitKey(1);
	}
}

/*
Returns the value of member 'm_img'.
*/
const cv::Mat& CvImageGUI::getImg() const
{
    return m_img;
}

/*
Set the value of member 'm_img' to 'img'.
*/
void CvImageGUI::setImg(const cv::Mat& img)
{
    m_img = img;
}

/*
Returns the value of member 'm_windowName'.
*/
const std::string CvImageGUI::getWindowName() const
{
    return m_windowName;
}

}
