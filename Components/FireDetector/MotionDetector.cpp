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

#include "MotionDetector.h"


// Finds the movement based on the last 3 frames
// frame is the current frame
// binaryMotion is a bynary image with the motion regions
// Maximum deviation of the image, the higher the value, the more motion is allowed
void MotionDetector::collinsMotion(cv::Mat frame, cv::Mat &binaryMotion, int minDifference){
	binaryMotion = cv::Mat::zeros(frame.size(), CV_8UC1);

	// Take a new image
	prevFrame = currentFrame.clone();
	currentFrame = nextFrame.clone();
	if(frame.channels() == 3){
		cvtColor(frame, nextFrame, CV_RGB2GRAY);
	} else{
		nextFrame = frame;
	}

	cv::Mat result = nextFrame;
	// check for empty frame
	if (prevFrame.empty() || nextFrame.empty() || currentFrame.empty() || 
		prevFrame.rows != nextFrame.rows || prevFrame.rows != currentFrame.rows || currentFrame.rows != nextFrame.rows)
	{
		currentBinaryFrame = binaryMotion.clone();
		return;
	}
	// d1 and d2 for calculating the differences
	// result, the result of and operation, calculated on d1 and d2
	// numberOfChanges, the amount of changes in the result matrix.
	// color, the color for drawing the rectangle when something has changed.
	cv::Mat d1, d2;
	
	// Calc differences between the images and do AND-operation
	// threshold image, low differences are ignored (ex. contrast change due to sunlight)
	absdiff(prevFrame, nextFrame, d1);
	absdiff(nextFrame, currentFrame, d2);
	bitwise_and(d1, d2, binaryMotion);
	threshold(binaryMotion, binaryMotion, minDifference, 255, CV_THRESH_BINARY);

	currentBinaryFrame = binaryMotion.clone();
	//cout << "Bin " << binaryMotion.size() << "\n";

}

// gets change count
void MotionDetector::getChangeCount(cv::Mat &changes){
	changes = cv::Mat::zeros(currentBinaryFrame.size(), CV_8UC1);
	cv::Mat diff, tmp;
	//cout<< currentBinaryFrame.size() << "\n";
	normalize(currentBinaryFrame, tmp, 0, 1, cv::NORM_MINMAX, CV_8UC1);
	//se não tiver dados no frame atual ou no anterior então abandona o processamento
	if(currentBinaryFrame.rows < 1 || lastBinaryImage.rows < 1  || lastBinaryImage.rows != currentBinaryFrame.rows){
		lastBinaryImage = currentBinaryFrame.clone();
		lastBinaryFrames.clear();
		return;
	}
	
	//calcula a diferença em relação ao frame anterior e salva os valores computados
	absdiff(lastBinaryImage, tmp, diff);
	
	//salva para a próxima execução
	lastBinaryFrames.push_back(diff);
	lastBinaryImage = tmp;

	//controla o tamanho do vetor
	if(lastBinaryFrames.size() > historySize){
		lastBinaryFrames.erase(lastBinaryFrames.begin()+1);
	}
	
	//soma todos os dados de diferença do vetor
	for( auto f : lastBinaryFrames){
		changes = changes + f;
	}
	
}


void MotionDetector::contoursMotion(cv::Mat frame, cv::Mat &binaryMotion, int minDifference){
	binaryMotion = cv::Mat::zeros(frame.size(), CV_8UC1);

	// Take a new image
	prevFrame = currentFrame.clone();
	currentFrame = nextFrame.clone();
	if(frame.channels() == 3){
		cvtColor(frame, nextFrame, CV_BGR2GRAY);
	} else{
		nextFrame = frame.clone();
	}

	//Get the contours
	cv::GaussianBlur(nextFrame, nextFrame, cv::Size(9,9), 0.8);
	cv::Laplacian( nextFrame, nextFrame, CV_8UC1, 1);

	cv::Mat result = nextFrame;
	// check for empty frame
	if (prevFrame.empty() || nextFrame.empty() || currentFrame.empty()){
		currentBinaryFrame = binaryMotion.clone();
		return;
	}
	
	// Calc differences between the images
	// threshold image, low differences are ignored (ex. contrast change due to sunlight)
	absdiff(nextFrame, currentFrame, binaryMotion);
	threshold(binaryMotion, binaryMotion, minDifference, 255, CV_THRESH_TOZERO);

	currentBinaryFrame = binaryMotion.clone();
	//cout << "Bin " << binaryMotion.size() << "\n";

}

// gets change count
void MotionDetector::getChangeAbsDiff(cv::Mat &changes){
	changes = cv::Mat::zeros(currentBinaryFrame.size(), CV_8UC1);
	cv::Mat diff, tmp;
	//se não tiver dados no frame atual ou no anterior então abandona o processamento
	if(currentBinaryFrame.rows < 1){
		return;
	}
	
	//salva para a próxima execução
	lastBinaryFrames.push_back(currentBinaryFrame);

	//controla o tamanho do vetor
	if(lastBinaryFrames.size() > historySize){
		lastBinaryFrames.erase(lastBinaryFrames.begin()+1);
	}
	
	//soma todos os dados de diferença do vetor
	for( auto f : lastBinaryFrames){
		changes = changes + f;
	}
	cv::Mat k = cv::getGaussianKernel(5, .7);
	cv::erode(changes, changes, k);
}


void MotionDetector::frameDiff(cv::Mat frame, cv::Mat &binaryMotion, int minDifference){
	binaryMotion = cv::Mat::zeros(frame.size(), CV_8UC1);

	// Take a new image
	prevFrame = currentFrame.clone();
	currentFrame = nextFrame.clone();
	if(frame.channels() == 3){
		cvtColor(frame, nextFrame, CV_RGB2GRAY);
	} else{
		nextFrame = frame;
	}

	cv::Mat result = nextFrame;
	// check for empty frame
	if (prevFrame.empty() || nextFrame.empty() || currentFrame.empty()){
		currentBinaryFrame = binaryMotion.clone();
		return;
	}
	// d1 and d2 for calculating the differences
	// result, the result of and operation, calculated on d1 and d2
	// numberOfChanges, the amount of changes in the result matrix.
	// color, the color for drawing the rectangle when something has changed.
	cv::Mat d1, d2;
	
	// Calc differences between the images and do AND-operation
	// threshold image, low differences are ignored (ex. contrast change due to sunlight)
	absdiff(prevFrame, nextFrame, d1);
	absdiff(nextFrame, currentFrame, d2);
	bitwise_and(d1, d2, binaryMotion);
	threshold(binaryMotion, binaryMotion, minDifference, 255, CV_THRESH_TOZERO);

	currentBinaryFrame = binaryMotion.clone();

}

//Constructor
MotionDetector::MotionDetector(void)
{
	historySize = 10;
}

//Destructor
MotionDetector::~MotionDetector(void)
{
}
