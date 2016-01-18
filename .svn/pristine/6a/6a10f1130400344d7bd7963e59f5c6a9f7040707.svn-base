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

//Main include
#include "VisionImplementationCv.h"
#include "ContourSmooth.h"
#include "TemporalDataExtractor.h"
#include "C45Helper.h"

#ifndef _FIRETRACKERCHENEBERT_H
#define _FIRETRACKERCHENEBERT_H

#pragma once

class FireTrackerChenebert
    : public VisionCore::Tracker<cv::Mat, std::vector<std::vector<cv::Point>>>
{
private:

	std::vector<std::vector<cv::Point>> m_currentTrack;
	bool m_lostTrack;

	/// motion
	cv::Mat motion;

	// 
	cv::Mat last_frame;

	/// store the last frame data
	cv::Mat last_binary;


	/// smooth the contours 
    ContourSmooth cs;

	/// get the data for temporal analysis
    TemporalDataExtractor tde;


	// check contour roundness using Borges 2010 approach
	// Border Roundness = P_{S}/ P_{CHS}
	float getRoundness(std::vector<cv::Point> &in);

	/// find image median
    cv::Scalar getMedian(cv::Mat &image, cv::Mat &mask);

	/// Get texture Features as proposed in
	/// A NON-TEMPORAL TEXTURE DRIVEN APPROACH TO REAL-TIME FIRE DETECTION
	/// Audrey Chenebert, Toby P. Breckon and Anna Gaszczak ICIP 2011
	/// 10 bins histogram for H
	/// 10 bins histogram for S
	/// GLCM  Energy, Entropy, Contrast, Homogenity and Correlation
	void getTextureFeatures(cv::Mat &image, std::vector<std::vector<cv::Point>> &in);

	
public:
	/// Get the last track
    const std::vector<std::vector<cv::Point>>& getLastTrack();

	/// Random Forests
    C45Helper region_classifier;


	/// True if no fire is detected
    bool lostTrack() const;

	/// Given an image (i.e. video frame), update the tracked objects.
    void update(const VisionCore::Frame<cv::Mat>& frame);

	/// stores the color classified regions on the last frame
	cv::Mat color_classified;

	/// vector with the statistics for each contour
	std::vector<std::vector<float>> spatial_data;

	/// temporary contour store for the current frame
	std::vector<std::vector<cv::Point>> current_contours;

	// is the video from a static camera?
	bool static_video;

	//How many frames do we use for processing the temporal properties?
	int frame_rate;

	//Show debug info?
	bool show_debug_info;

	/// Only extract statistics!!! Set as true only if you're trying to generate training data.
	/// By setting this parameter as true you can access the statistics.
	/// You can easily get the data on 'spatial_data' and 'current_contours' or call the 'generateRegionTrainingData' function
	bool dont_classify_by_region_stats;

	/// Create a new tracker
	/// As parameters you should provide trained classification models. Trained models are provided on the package.
	/// If you don't have them now, you can create them using the RandomForestHelper Class
	/// The Region Classification Model takes 14 attributes + a class where 1 is equal fire and 0 is non fire
	/// The parameters are given in the following order:
	///		0 - area
	///		1 - roundness
	///		2,3,4 - mean
	///		5,6,7 - median
	///		8,9,10 - standard deviation
	///		11,12,13 - skewness
	/// The Color Classification Model should be created considering RGB and HSV pixel values + a binary class that states the if it represents fire (1) or not (0).
	/// The parameters are given in the following order:
	///		0,1,2 - B, G, R
	///		3,4,5 - H, S, V
	///
	/// You can also build your own models by accessing the rfh_color and rfh_region public attributes.
	/// Ex:
	///		rfh_region.params.max_depth = 10;
	///		rfh_region.params.min_sample_count = 50; 
	///		rfh_region.readTrainDataFromCsv("region_training_data3.csv", ',', true, false);
	///		rfh_region.trainClassifier();
	/// You can save the generated classification model using:
	///		rfh_region.saveClassifier("region_classification_model");
	/// Further information about how the Random Forests are supposed to work can be found in Breiman 2001.
	FireTrackerChenebert(std::string region_classification_model);

	/// Destructor
	~FireTrackerChenebert(void);

	/// This is method used to extract region training data. 
	/// Before you use this, you have to set the 'dont_classify_by_region_stats' to false
	/// As it is a task that is very hard to do manually we created it so that you can easilly validate the contours against the groud truth.
	/// appending the class attribute and returning a string with good and bad contours.
	/// By good contours we assume the ones where the similarity is greater than 0.8.
	/// Bad bad contours we assum the ones that do not have any intersection with the ground truth.
	/// Usage example:
	///		for(auto c: tracker.generateRegionTrainingData(simFc, gt.getFrameAt(frame.getNumber()).rectOnFrame)){
	///			for(auto v: c){
	///				export_data.append(to_string(v) + ";");
	///			}
	///			export_data.append("\n");
	///		}
    std::vector<std::vector<float>> generateRegionTrainingData(std::function<double(std::vector<cv::Point> &dt, cv::Rect &gt )> simFcn, std::vector<cv::Rect> groundTruth);
};

#endif
