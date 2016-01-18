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

#include "FrameAnnotation.h"

//Methods for handle the frame annotations

void FrameAnnotation::addRect(cv::Rect r){
	rectOnFrame.push_back(r);
}


FrameAnnotation::FrameAnnotation(int frameNum, std::vector<cv::Rect> rects){
	frameNumber = frameNum;
	rectOnFrame = rects;
}


/**
 * Merge a set of rectangles if there's an overlap between each rectangle for more than specified overlap area
 * @param   overlap the minimum area of overlap before 2 rectangles are merged
 * @param   group_threshold only the rectangles that have more than the remaining group_threshold rectangles will be retained
 **/
void FrameAnnotation::mergeRectangles( float overlap, unsigned group_threshold ) {
    std::vector<cv::Rect> output;
    std::vector<cv::Rect> intersected;
    std::vector< std::vector<cv::Rect> > partitions;

    while( rectOnFrame.size() > 0 ) {
        cv::Rect a      = rectOnFrame[rectOnFrame.size() - 1];
        int a_area  = a.area();
		rectOnFrame.pop_back();

        if( partitions.empty() ) {
            std::vector<cv::Rect> vec;
            vec.push_back( a );
            partitions.push_back( vec );
        }
        else {
            bool merge = false;
            for( unsigned i = 0; i < partitions.size(); i++ ){

                for( unsigned j = 0; j < partitions[i].size(); j++ ) {
                    cv::Rect b = partitions[i][j];
                    int b_area = b.area();

                    cv::Rect intersect = a & b;
                    int intersect_area = intersect.area();

                    if (( a_area == b_area ) && ( intersect_area >= overlap * a_area  ))
                        merge = true;
                    else if (( a_area < b_area ) && ( intersect_area >= overlap * a_area  ) )
                        merge = true;
                    else if (( b_area < a_area ) && ( intersect_area >= overlap * b_area  ) )
                        merge = true;

                    if( merge )
                        break;
                }

                if( merge ) {
                    partitions[i].push_back( a );
                    break;
                }
            }

            if( !merge ) {
                std::vector<cv::Rect> vec;
                vec.push_back( a );
                partitions.push_back( vec );
            }
        }
    }

    for( unsigned i = 0; i < partitions.size(); i++ ) {
        if( partitions[i].size() <= group_threshold )
            continue;

        cv::Rect merged = partitions[i][0];
        for( unsigned j = 1; j < partitions[i].size(); j++ ) {
            merged |= partitions[i][j];
        }

        output.push_back( merged );

    }

	rectOnFrame = output;
}

//Default Constructor and destructor
FrameAnnotation::FrameAnnotation(void)
{
}


FrameAnnotation::~FrameAnnotation(void)
{
}
