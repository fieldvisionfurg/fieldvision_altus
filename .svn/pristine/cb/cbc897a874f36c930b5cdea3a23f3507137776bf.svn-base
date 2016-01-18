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

#ifndef VISIONHELPERCV
#define VISIONHELPERCV

#include "VisionCore.h"

namespace VisionCore{
namespace Abstractions{

template<>
inline std::string VisionHelpper::getAsCsvString(const cv::Rect& obj){
    std::stringstream ss;
    ss << obj.x << "," << obj.y << "," << obj.width << "," <<  obj.height;
    return ss.str();
}


template<>
inline std::string VisionHelpper::getAsCsvString(const cv::Point& obj){
    std::stringstream ss;
    ss << obj.x << "," << obj.y ;
    return ss.str();
}

template<>
inline std::string VisionHelpper::getAsCsvString(const std::map<long,cv::Rect>& obj){
    std::stringstream ss;
    ss << obj.size() << ",";
    for(std::pair<long,cv::Rect> p : obj){
        ss << p.first << ",";
        ss << VisionHelpper::getAsCsvString(p.second) << "," ;
    }
    return ss.str();
}

template<>
inline std::string VisionHelpper::getAsCsvString(const double& obj){
    std::stringstream ss;
    ss << obj;
    return ss.str();
}

template<>
inline std::string VisionHelpper::getAsCsvString(const bool& obj){
    if(obj)
        return "1";
    else
        return "0";
}

template<>
inline double VisionHelpper::similarity(const cv::Rect &r1,const cv::Rect &r2){
    double areaR1=r1.area();
    double areaR2=r2.area();
    double areaIntrsect=(r1 & r2).area();
    double similarity = areaIntrsect / (areaR1+areaR2-areaIntrsect);
    return similarity;
}

template<>
inline double VisionHelpper::similarity(const std::vector<cv::Point> &r1,const std::vector<cv::Point> &r2){
    int minXr1=std::numeric_limits<int>::max();
    int maxXr1=-1;
    int minYr1=std::numeric_limits<int>::max();
    int maxYr1=-1;
    int minXr2=std::numeric_limits<int>::max();
    int maxXr2=-1;
    int minYr2=std::numeric_limits<int>::max();
    int maxYr2=-1;

    //find min and max of each region so we only focus on area of interest
    for(const cv::Point& p : r1){
        if(p.x < minXr1)
            minXr1=p.x;
        if(p.x > maxXr1)
            maxXr1=p.x;
        if(p.y < minYr1)
            minYr1=p.y;
        if(p.y > maxYr1)
            maxYr1=p.y;
    }

    for(const cv::Point& p : r2){
        if(p.x < minXr2)
            minXr2=p.x;
        if(p.x > maxXr2)
            maxXr2=p.x;
        if(p.y < minYr2)
            minYr2=p.y;
        if(p.y > maxYr2)
            maxYr2=p.y;
    }

    //create minimum size images containing both regions
    int maxX=std::max(maxXr1,maxXr2);
    //    int minX=std::min(minXr1,minXr2);
    int maxY=std::max(maxYr1,maxYr2);
    //    int minY=std::min(minYr1,minYr2);
    int width=maxX;  //could be maxX-minX
    int heigth=maxY;
    cv::Mat img1 = cv::Mat::zeros(heigth,width,CV_8UC1);
    cv::Mat img2 = cv::Mat::zeros(heigth,width,CV_8UC1);

    //create images with filled contours
    std::vector<std::vector<cv::Point>> contours;
    contours.push_back(r1);
    cv::drawContours(img1,contours,-1,255,CV_FILLED);
    contours.clear();
    contours.push_back(r2);
    cv::drawContours(img2,contours,-1,255,CV_FILLED);

    //compute intersection and similarity
    cv::Mat intersect;
    cv::bitwise_and(img1,img2,intersect);
    int area1=cv::countNonZero(img1);
    int area2=cv::countNonZero(img2);
    double intersectArea = cv::countNonZero(intersect);

    double similarity = intersectArea / (area1+area2-intersectArea);
    return similarity;
}

}
}

#endif // VISIONHELPERCV

