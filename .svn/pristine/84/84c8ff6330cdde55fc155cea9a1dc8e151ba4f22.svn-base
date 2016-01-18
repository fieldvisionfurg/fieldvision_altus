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

#include "VideoAnnotation.h"

// This would return the copy of time/date in std::string object to caller
std::string get_gmtime_asctime() {
 time_t ltime;
 time(&ltime);
 tm* gt=new tm; ::gmtime_s(gt,&ltime);
 char tmp[32]; ::asctime_s(tmp,32,gt);
 std::string output(tmp);
 return output;
}

void VideoAnnotation::addFrame(FrameAnnotation fa){
	frames.push_back(fa);
}

FrameAnnotation VideoAnnotation::getFrameAt(unsigned frameNumber){
	if(frames.size() > frameNumber){
	return frames.at(frameNumber);
	}else{
		return FrameAnnotation();
	}
}

FrameAnnotation VideoAnnotation::getNextFrame(){
	currentIndex ++;
	return frames.at(currentIndex);
}

FrameAnnotation VideoAnnotation::getCurrentFrame(){
	return frames.at(currentIndex);
}

void VideoAnnotation::saveToFile(std::string fileName){
	//loads video annotation from file
	cv::FileStorage fs(fileName, cv::FileStorage::WRITE);

	fs << "fileName" << videoFileName;
	fs << "frameRate" << frameRate;
	fs << "shapeType" << shape;
	fs << "frameWidth" << frameWidth;
	fs << "frameHeight" << frameHeight;


    std::string dt = get_gmtime_asctime();
	fs << "dataSetDate" << dt;

	fs << "frames" << "[";

	for(FrameAnnotation &i : frames )
	{
		fs << "{:";
		fs << "frameNumber" <<i.frameNumber;
		fs << "annotations" << "[";
		for( auto &j : i.rectOnFrame ){
			fs << "[" << j.x << j.y << j.width << j.height << "]";
		}
		fs << "]";
		fs << "}";
	}

	fs << "]";
	fs.release();
	printf("Saving annotations to file ... Done!\n");
}

void VideoAnnotation::loadFromFile(std::string fileString = "keypoints.xml") {
	cv::FileStorage fs(fileString, cv::FileStorage::READ);
	if(fs.isOpened())
	{

		cv::read(fs["fileName"], videoFileName, ""); 
		cv::read(fs["frameRate"], frameRate, -1);
		cv::read(fs["shapeType"], shape, "");
		cv::read(fs["frameWidth"], frameWidth, 0);
		cv::read(fs["frameHeight"], frameHeight, 0);

		cv::FileNode framesNode = fs["frames"];
		cv::FileNodeIterator itf = framesNode.begin(), itf_end = framesNode.end();

		// iterate through a sequence using FileNodeIterator
		for(int idx = 0; itf != itf_end; ++itf, idx++ )
		{
			FrameAnnotation fa;
			fa.frameNumber = (int) (*itf)["frameNumber"];

			cv::FileNode annotationNode = (*itf)["annotations"];
			cv::FileNodeIterator ita = annotationNode.begin(), ita_end = annotationNode.end();
			//std::cout << annotationNode.size();
			//returns the annotated points
			for(int i = 0; ita != ita_end; ++ita, i++ )
			{
				cv::Rect rect;
				*ita >> rect;
				fa.rectOnFrame.push_back(rect);
			}
			//add current frame data on framelist
			frames.push_back(fa);
            frameCount = frames.size();
		} 
    }

	fs.release();
}

//Default constructor and destructor
VideoAnnotation::VideoAnnotation(void) throw()
{
	currentIndex = 0;frameWidth = 0; frameHeight = 0; frameCount = 0;
}


VideoAnnotation::~VideoAnnotation(void)
{
}
