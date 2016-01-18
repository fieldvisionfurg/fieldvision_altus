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


#ifndef _ARTAGDETECTORBLP_H
#define _ARTAGDETECTORBLP_H

#include "VisionImplementationCv.h"

namespace Viscv
{

/// Defines a generic polygon with fixed number of vertices.
template<typename T = int , unsigned int NumVertices = 4>
class Polygon
{
public:
	cv::Point vertices[NumVertices];
};

/// Defines a augmented reality tag, which contains a bounding polygon and a tag ID.
class ARTag
{
public:
    /// Polygon that represents the external contour of a tag.
	Polygon<int,4> box;
    /// Tag type (ID).
	std::string tagID;
	/// Construtor
	ARTag(Polygon<int,4> box_,std::string tagID_)
		: box(box_)
        , tagID(tagID_)
    {

    }

};


/// Implements a augmented reality tag detector.
/**
 * This algorith is similar to the one implemented in ARToolkit.
 * It first detects a bounding polygon around a tag. Then it extract the image inside the tag and
 * match it agains a set of enrolled tags (see enrollTag method), assigning the tag with higher similarity.
 *
 * \ingroup Viscv
*/
class ARTagDetectorBLP
    : public VisionCore::Detector<cv::Mat,ARTag>
{
public:
    /// Defult constructor.
	ARTagDetectorBLP();

	/// Destructor
	~ARTagDetectorBLP();

    std::vector<ARTag> detect(const cv::Mat& img) const;

    /// Enrolls new tags.
	void enrollTag(const cv::Mat& tagImg,const std::string& tagName);

    /// Tag border size.
	int borderSize;

    /// Threshold used in image binarization.
	int binarizationThreshold;

private:
    /// List with the enrolled tags that can be recognized. Each tag has an example image and a name (tag ID).
	std::list<std::pair<cv::Mat,std::string>> enrolledTags;

    /// Standard tag size with border
	int tagHeight;

	int tagWidth;

    /// Detects the tag external contour as a polygon.
	std::vector<Polygon<int,4>> detectTagBox(const cv::Mat& img) const;

    /// Extracts the tag internal image as a frontal image. Uses perspective transformation.
	void extractCoreTag(const cv::Mat& img,const Polygon<int,4>& tagBox, cv::Mat& tagImgOut) const;

    /// Matches a image with a tag core.
	double matchTag(const cv::Mat& img,const cv::Mat& tagCoreImg) const ;

};
}
#endif // ndef _ARTAGDETECTORBLP_H
