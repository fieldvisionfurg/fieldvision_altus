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

#include "FireTrackerChenebert.h"
#include "TextureGLCM.h"

/// Lost track
inline bool FireTrackerChenebert::lostTrack() const
{
	if(m_currentTrack.empty())
		return true;
	return false;
}

/// Get last track
const std::vector<std::vector<cv::Point>>& FireTrackerChenebert::getLastTrack()
{

	return m_currentTrack;
}

/// Update tracker
void FireTrackerChenebert::update(const VisionCore::Frame<cv::Mat>& frame) {
	m_currentTrack.clear();
	spatial_data.clear();
	current_contours.clear();

	cv::Mat img = frame.getImg();
	color_classified = cv::Mat::zeros(img.rows, img.cols, CV_8UC1);

	/// motion
	if(img.size() == last_frame.size()){
		cv::absdiff(img, last_frame, motion);
		cv::cvtColor(motion, motion, CV_BGR2GRAY);
		cv::threshold(motion, motion, 5, 255, CV_THRESH_BINARY);
		cv::erode(motion, motion, cv::Mat::ones(cv::Size(2,2), CV_8UC1));
	}else{
		motion = cv::Mat::zeros(img.size(), CV_8UC1);
	}
	last_frame = img.clone();


	/// color segmentation
	cv::Mat img_hsv;
	cv::cvtColor(img, img_hsv, CV_BGR2HSV);

	std::vector<std::vector<float>> channels_hsv, channels_rgb;
    TemporalDataExtractor tde;
	/// get the channels as vector

	std::vector<cv::Mat> planes, hsvPlanes;
	//Copy each color to his own array
	cv::split(img, planes); 
	cv::split(img_hsv, hsvPlanes);
	unsigned S_T = 60, R_T = 125;
	//for each row in the image
#pragma loop(hint_parallel(0))
	for( int y = 0; y < img.rows; y++ )
	{
		uchar* ptr_b = planes[0].ptr<uchar>(y);
		uchar* ptr_g = planes[1].ptr<uchar>(y);
		uchar* ptr_r = planes[2].ptr<uchar>(y);
		uchar* ptr_s = hsvPlanes[1].ptr<uchar>(y);

		uchar *ptr_bin = color_classified.ptr<uchar>(y);
		//for each column in the image
#pragma loop(hint_parallel(0))
		for( int x = 0; x < img.cols; x++ )
		{
			if (ptr_r[x] >= R_T &&
				ptr_r[x] >= ptr_g[x] &&
				ptr_g[x] > ptr_b[x] &&
				ptr_s[x] >= ((255 - ptr_r[x]) * S_T)/R_T )
				ptr_bin[x] = 255;
		}
	}


	color_classified.convertTo(color_classified, CV_8UC1);
	//END of color classification

	/// filter contours based on roundness
	std::vector<std::vector<cv::Point> > contours;
	std::vector<cv::Vec4i> hierarchy;
	cv::Mat canny_output = color_classified.clone();
	cv::findContours( canny_output, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);

	/// get variance and skewness
	//getStatistics(img, contours);
	getTextureFeatures(img, contours);

	if(!dont_classify_by_region_stats){
		if(spatial_data.size()>0){
			cv::Mat region_data = cv::Mat::zeros(spatial_data.size(), spatial_data[0].size(), CV_32F);
			for(int i = 0; i< region_data.rows; i++){
				float *p = region_data.ptr<float>(i);
				for(unsigned j = 0; j < spatial_data[0].size(); j++)
					p[j] = spatial_data[i][j];
			}

			std::vector<std::vector<cv::Point>> tmp_c;
			std::vector<std::vector<float>> tmp_stat;
			for(int i = 0; i< region_data.rows; i++){
				//each region is classified using the classifier
                cv::Mat r = region_data.row(i);
                if(region_classifier.classify(r) > 0.0){
					tmp_c.push_back(current_contours[i]);
					tmp_stat.push_back(spatial_data[i]);
				}
			}
			current_contours = tmp_c; spatial_data = tmp_stat;
		}

		/// smooth resulting contours
        m_currentTrack = cs.smoothContoursP(current_contours, .01f);

	}
	else{
		m_currentTrack = current_contours;
	}
	/// show debug info
	if(show_debug_info){
		cv::imshow("motion", motion);
		cv::normalize(color_classified,color_classified,0,255, cv::NORM_MINMAX);
		cv::imshow("color segmentation", color_classified);
	}
}


/// A probabilistic approach for vision-based fire detection in videos
/// Borges, Paulo Vinicius Koerich and Izquierdo, Ebroul - 2010
/// Border Roundness = P_{S}/ P_{CHS}
float FireTrackerChenebert::getRoundness(std::vector<cv::Point> &in){
	std::vector<std::vector<cv::Point>> out;

	// Find the convex hull object for each contour
	std::vector<cv::Point> convex_hull;
	convexHull( cv::Mat(in), convex_hull, false ); 

	/// Calculates a contour perimeter or a curve length.
	/// contour perimeter / convex_hull perimeter
    float br = static_cast<float>(cv::arcLength(cv::Mat(in), true) / cv::arcLength(cv::Mat(convex_hull), true));

	/// return
	return br;
}

/// Get texture Features as proposed in
/// A NON-TEMPORAL TEXTURE DRIVEN APPROACH TO REAL-TIME FIRE DETECTION
/// Audrey Chenebert, Toby P. Breckon and Anna Gaszczak ICIP 2011
/// 10 bins histogram for H
/// 10 bins histogram for S
/// GLCM  Energy, Entropy, Contrast, Homogenity and Correlation
void FireTrackerChenebert::getTextureFeatures(cv::Mat &image, std::vector<std::vector<cv::Point>> &in){
	// Cria o objeto textura
	TextureGLCM* texture = new TextureGLCM( );
	TextureGLCM::GLCM* glcm;

	for( unsigned i = 0; i< in.size(); i++ ){ // for each contour
		if(cv::contourArea(in[i])> 150){ //very small areas are ignored
			/// for each contour we look for
			/// 0 - 10 -> H histogram
			/// 11 - 20 -> S histogram
			/// 21 -> GLCM Energy, 
			/// 22 -> GLCM Entropy,
			/// 23 -> GLCM Contrast, 
			/// 24 -> GLCM Homogenity
			/// 25 -> GLCM Correlation
			std::vector<float> contour_data; contour_data.resize(30);


			//get data
			cv::Rect roi = cv::boundingRect(in[i]);
			//create mask
			cv::Mat mask = cv::Mat::zeros(cv::Size(image.cols, image.rows), CV_8UC1);
			drawContours( mask, in, i, cv::Scalar::all(1), -1);
			//crop areas
			mask = mask(roi);
			cv::Mat the_region = image(roi).clone();

			std::vector<cv::Mat> hsv_channels;
			cv::split(the_region, hsv_channels); 

			for(unsigned c=0; c < 2; c++){ //for each channel
				cv::Mat cur_img  = hsv_channels[c].clone(); cur_img.convertTo(cur_img, mask.type());
				int histSize[] = {10};	float sranges[] = { 0, 256 }; const float* ranges[] = {  sranges };
				cv::Mat hist;
				// we compute the histogram from the 0-th and 1-st channels
				int channels[] = {0};
				calcHist( &cur_img, 1, channels, mask, 
					hist, 1, histSize, ranges,
					true, // the histogram is uniform
					false );

				hist = hist / roi.area(); //normalize
				// uses 10 bins
				for (unsigned j=0; j < 10; j++)
					contour_data[10*c + j] = hist.at<float>(cv::Point(0,j));

				//GLCM	completa um vetor com 30 variáveis
				hsv_channels[c].convertTo(hsv_channels[c], CV_8UC1);
                IplImage hsv_c=hsv_channels[c];
                IplImage* pGray = cvCloneImage(&hsv_c);

				//Direção da Textura
				const int StepDirections[] = { 0,1,-1,1,-1,0,-1,-1 };

				// buid the GLCM
				glcm = texture->CreateGLCM(pGray, 4, StepDirections, 1, CV_GLCM_OPTIMIZATION_LUT); 
				//// get the features from GLCM
				texture->CreateGLCMDescriptors(glcm, CV_GLCMDESC_OPTIMIZATION_ALLOWDOUBLENEST); 

                contour_data[20 + 5*c] = static_cast<float>(texture->GetGLCMDescriptor(glcm, 0, CV_GLCMDESC_ENERGY));
                contour_data[21 + 5*c] = static_cast<float>(texture->GetGLCMDescriptor(glcm, 0, CV_GLCMDESC_ENTROPY));
                contour_data[22 + 5*c] = static_cast<float>(texture->GetGLCMDescriptor(glcm, 0, CV_GLCMDESC_CONTRAST));
                contour_data[23 + 5*c] = static_cast<float>(texture->GetGLCMDescriptor(glcm, 0, CV_GLCMDESC_HOMOGENITY));
                contour_data[24 + 5*c] = static_cast<float>(texture->GetGLCMDescriptor(glcm, 0, CV_GLCMDESC_CORRELATION));

				cvReleaseImage(&pGray);
				texture->ReleaseGLCM(&glcm, CV_GLCM_ALL);
				//FIM GLCM
			}

			//adiciona dados nas estatístisticas
			spatial_data.push_back(contour_data);
			current_contours.push_back(in[i]);

			if (show_debug_info)
				for (unsigned att_count = 0; att_count < contour_data.size(); att_count++){
					std::cout << att_count <<" = " <<contour_data[att_count] << " | ";
				}

		}//endif
	}//end contours for

	delete texture;
}



/// Uses the hstogram in order to find the median value
/// Has  better performance than compute the median by sorting the values
cv::Scalar FireTrackerChenebert::getMedian(cv::Mat &image, cv::Mat &mask){

	cv::Scalar out;
	std::vector<cv::Mat> img_channels;
	cv::split(image, img_channels); 

	for(unsigned i=0; i < img_channels.size(); i++){ //for each channel
		cv::Mat cur_img  = img_channels[i].clone(); cur_img.convertTo(cur_img, mask.type());
		int histSize[] = {256};	float sranges[] = { 0, 256 }; const float* ranges[] = {  sranges };
		cv::Mat hist;
		// we compute the histogram from the 0-th and 1-st channels
		int channels[] = {0};
		calcHist( &cur_img, 1, channels, mask, 
			hist, 1, histSize, ranges,
			true, // the histogram is uniform
			false );

		long sum = 0;	int bin = 0;
		cv::bitwise_and(cur_img, mask, cur_img);
		cv::Scalar maxElements = cv::sum(cur_img);
		// the bean where the midlle of the values is reached is the center point (or median)
		while((sum < maxElements[0]/2) && (bin < 255))
		{
			sum += (long) hist.at<float>(bin);
			bin ++;
		}
		out[i] = bin;
	}
	//return the median
	return out;
}

/// Create a new tracker
/// As parameters you should provide trained classification models. Trained models are provided on the package.
/// If you don't have them now, you can create them using the RandomForestHelper Class
/// The Region Classification Model takes 30 attributes + a class where 1 is equal fire and 0 is non fire
/// The parameters are given in the following order:
///
/// You can also build your own models by accessing the rfh_color and region_classifier public attributes.
/// Ex:
///		region_classifier.params.max_depth = 10;
///		region_classifier.params.min_sample_count = 50; 
///		region_classifier.readTrainDataFromCsv("region_training_data3.csv", ',', true, false);
///		region_classifier.trainClassifier();
/// You can save the generated classification model using:
///		region_classifier.saveClassifier("region_classification_model");
/// Further information about how the Random Forests are supposed to work can be found in Breiman 2001.
FireTrackerChenebert::FireTrackerChenebert(std::string region_classification_model){
	frame_rate = 29; static_video = false; m_lostTrack = true; show_debug_info = false; dont_classify_by_region_stats = false;

    if(!region_classification_model.empty())
        region_classifier.loadClassifier(region_classification_model);

}

/// Destructor
FireTrackerChenebert::~FireTrackerChenebert(void)
{
}

/// This is a helper method used to generate region training data
/// As it is a task that is very hard to do manually we created it so that you can easilly validate the contours against the groud truth
/// appending the class attribute and returning a string with good and bad contours
/// By good contours we assume the ones where the similarity is greater than 0.8.
/// Bad bad contours we assum the ones that do not have any intersection with the ground truth
std::vector<std::vector<float>> FireTrackerChenebert::generateRegionTrainingData(std::function<double(std::vector<cv::Point> &dt, cv::Rect &gt)> simFcn, std::vector<cv::Rect> groundTruth){
	std::vector<std::vector<float>> the_validated_contour;
	//get the good contours for training
	for(unsigned i = 0; i < current_contours.size(); i ++ ){
		for(cv::Rect r: groundTruth){
			if(simFcn(current_contours[i], r) > .8){//is in the gt
				std::vector<float> data = spatial_data[i];
				//if(data[4]>80.0){ //red channel
				data.push_back((float)1);
				the_validated_contour.push_back(data);
				break;
				//}
			}
		}
	}
	//get the bad contours for training
	for(unsigned i = 0; i < current_contours.size(); i ++ ){
		bool has_intersection = false;
		for(cv::Rect r: groundTruth){
			if(simFcn(current_contours[i], r) > 0){//is out of the gt
				has_intersection = true;
				break;
			}
		}
		if(!has_intersection){
			std::vector<float> data = spatial_data[i];
			data.push_back((double)0);
			the_validated_contour.push_back(data);
		}
	}
	return the_validated_contour;
}
