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

#include "FireTrackerSA.h"
#include "TextureGLCM.h"

/// Lost track
inline bool FireTrackerSA::lostTrack() const
{
	if(m_currentTrack.empty())
		return true;
	return false;
}

/// Get last track
const std::vector<std::vector<cv::Point>>& FireTrackerSA::getLastTrack()
{

	return m_currentTrack;
}

/// Update tracker
void FireTrackerSA::update(const VisionCore::Frame<cv::Mat>& frame) {
	m_currentTrack.clear();
	spatial_data.clear();
	current_contours.clear();
	cv::Mat img = frame.getImg();

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
	/// convert everything to float
	img.convertTo(img, CV_32FC3);
	img_hsv.convertTo(img_hsv, CV_32FC3);
    TemporalDataExtractor tde;
	/// get the channels as vector
	channels_hsv = tde.mat2vector(img_hsv);
	channels_rgb = tde.mat2vector(img);
	/// create the color test matrix
	cv::Mat color_data = cv::Mat::zeros(img_hsv.rows*img_hsv.cols, 6, CV_32F); //6 is bgr + hsv

	for(int i = 0; i< color_data.rows; i++){
		float *p = color_data.ptr<float>(i);
		p[0] = channels_rgb[0][i]; //blue
		p[1] = channels_rgb[1][i]; //green
		p[2] = channels_rgb[2][i]; //red
		p[3] = channels_hsv[0][i]; //hue
		p[4] = channels_hsv[1][i]; //saturation
		p[5] = channels_hsv[2][i]; //value
	}

	/// apply the color classifier
	std::vector<float> color_predictions;
	color_predictions.resize(color_data.rows);


	for(int i = 0; i< color_data.rows; i++){
		/// each pixel is classified using the classifier
        cv::Mat r = color_data.row(i);
        color_predictions[i] = (float) rfh_color.classify(r);
	}

	color_classified = tde.vector2Mat(color_predictions, img.rows, img.cols);
	color_classified.convertTo(color_classified, CV_8UC1);
	//END of color classification

	/// filter contours based on roundness
	std::vector<std::vector<cv::Point> > contours;
	std::vector<cv::Vec4i> hierarchy;
	cv::Mat canny_output = color_classified.clone();
	cv::findContours( canny_output, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);

	/// get variance and skewness
	getStatistics(img, contours);
	//getTextureFeatures(img, contours);

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
                if(rfh_region.classify(r) > 0.0){
					tmp_c.push_back(current_contours[i]);
					tmp_stat.push_back(spatial_data[i]);
				}
			}
			current_contours = tmp_c; spatial_data = tmp_stat;
		}


		/// match the current data with the last frame data
		cv::Mat current_binary = cv::Mat::zeros(frame.getImg().size(), CV_8UC1);
		for(unsigned i = 0; i < current_contours.size() ; i++)
			cv::drawContours(current_binary, current_contours, i, cv::Scalar::all(1), -1);

		if(current_binary.size() == last_binary.size()){
			cv::Mat tmp;
			std::vector<std::vector<cv::Point>> cmb_contours;
			cv::bitwise_and(current_binary, last_binary, tmp);
			cv::medianBlur(tmp, tmp, 5);
			cv::findContours( tmp, cmb_contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
			/// remove small contours and regions without movement
			for(auto c :cmb_contours){
				int area = (int) cv::contourArea(c);
				if(area > 100){
					cv::Scalar m = cv::sum( motion(boundingRect(c) ));
					if(m[0] > 0.2 * area)
						m_currentTrack.push_back(c);
				}
			}
			/// smooth resulting contours
            m_currentTrack = cs.smoothContoursP(m_currentTrack, .01f);
		}
		last_binary = current_binary;
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
float FireTrackerSA::getRoundness(std::vector<cv::Point> &in){
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
void FireTrackerSA::getTextureFeatures(cv::Mat &image, std::vector<std::vector<cv::Point>> &in){

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

				// Cria o objeto textura
				TextureGLCM* texture = new TextureGLCM( );
				TextureGLCM::GLCM* glcm;

				// buid the GLCM
				glcm = texture->CreateGLCM(pGray, 1, StepDirections, 4, CV_GLCM_OPTIMIZATION_LUT); 
				// get the features from GLCM
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
}


/// Get the statistics for each contour
/// Similar to: A probabilistic approach for vision-based fire detection in videos 
/// Borges, Paulo Vinicius Koerich and Izquierdo, Ebroul - 2010
void FireTrackerSA::getStatistics(cv::Mat &image, std::vector<std::vector<cv::Point>> &in){

	for( unsigned i = 0; i< in.size(); i++ ){ // for each contour
		cv::Rect roi = cv::boundingRect(in[i]);
		if(cv::contourArea(in[i])> 150 && roi.height > 3 && roi.width > 3){ //very small areas are ignored
			/// for each contour we look for
			/// 0 - area
			/// 1 - roundness
			/// 2,3,4 - mean BGR
			/// 5,6,7 - median BGR
			/// 8,9,10 - standard deviation BGR
			/// 11,12,13 - skewness BGR
			/// 14-to-22 - normalized sub region mean
			/// 23 - size proportions (cols/rows)
			std::vector<float> contour_data; contour_data.resize(24);

			float c_roundness;
            float c_area = static_cast<float>(cv::contourArea(in[i]));
			cv::Scalar c_mean, c_std_dev, c_median, c_skewness;

			c_roundness = getRoundness(in[i]);
			//get data
			//create mask
			cv::Mat mask = cv::Mat::zeros(cv::Size(image.cols, image.rows), CV_8UC1);
			drawContours( mask, in, i, cv::Scalar::all(1), -1);
			//crop areas
			mask = mask(roi);
			cv::Mat the_region = image(roi).clone();

			//extract dispersion data
			cv::meanStdDev(the_region, c_mean, c_std_dev, mask);

			// the median is used in order to compute the skewness
			// the way it is implemented here is using histograms in order to find the center
			// Assimetria de Pearson
			// mean less median/ std deviation
			c_median = getMedian(the_region, mask);

			c_skewness[0] = c_std_dev[0] > 0? 3*(c_mean[0]  - c_median[0]) / c_std_dev[0] : 0;
			c_skewness[1] = c_std_dev[1] > 0? 3*(c_mean[1]  - c_median[1]) / c_std_dev[1] : 0;
			c_skewness[2] = c_std_dev[2] > 0? 3*(c_mean[2]  - c_median[2]) / c_std_dev[2] : 0;

			// get the mean for now subregions
			cv::Scalar c_sr_mean, c_sr_std_dev;
			for(unsigned c = 0; c < 3; c++){
				for(unsigned r = 0; r < 3;r++){
				
					cv::Rect sr_roi(cvFloor(the_region.cols/3)*c, cvFloor(the_region.rows/3)*r, cvFloor(the_region.cols/3), cvFloor(the_region.rows/3));
					cv::Mat s_region = the_region(sr_roi).clone(), s_mask = mask(sr_roi).clone();
					cv::meanStdDev(s_region, c_sr_mean, c_sr_std_dev, s_mask);
					// red channel only
                    contour_data[14+(3*c)+r] = static_cast<float>(c_sr_mean[2]/c_mean[2]);
				}
			}
			//0 - area; 1 - roundness; 2,3,4 - mean; 5,6,7 - median; 8,9,10 - standard deviation; 11,12,13 - skewness
			contour_data[0] = c_area;
			contour_data[1] = c_roundness;
            contour_data[2] = static_cast<float>(c_mean[0]);		contour_data[3] = static_cast<float>(c_mean[1]);		contour_data[4] = static_cast<float>(c_mean[2]);
            contour_data[5] = static_cast<float>(c_median[0]);		contour_data[6] = static_cast<float>(c_median[1]);		contour_data[7] = static_cast<float>(c_median[2]);
            contour_data[8] = static_cast<float>(c_std_dev[0]);		contour_data[9] = static_cast<float>(c_std_dev[1]);		contour_data[10] = static_cast<float>(c_std_dev[2]);
            contour_data[11] = static_cast<float>(c_skewness[0]);	contour_data[12] = static_cast<float>(c_skewness[1]);	contour_data[13] = static_cast<float>(c_skewness[2]);
			contour_data[23] = (float)the_region.cols/(float)the_region.rows;
			//
			spatial_data.push_back(contour_data);
			current_contours.push_back(in[i]);
			if (show_debug_info)
				std::cout << "\nsigma = " << c_std_dev <<"\t skewness = " << c_skewness << "\t mean = " <<c_mean << "\tmedian = " <<c_median << "\tarea = " << c_area << "\troundness = " << c_roundness;

		}//endif
	}//end contours for

}

/// Uses the hstogram in order to find the median value
/// Has  better performance than compute the median by sorting the values
cv::Scalar FireTrackerSA::getMedian(cv::Mat &image, cv::Mat &mask){

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
FireTrackerSA::FireTrackerSA(std::string region_classification_model, std::string color_classification_model){
	frame_rate = 29; static_video = false; m_lostTrack = true; show_debug_info = false; dont_classify_by_region_stats = false;

    if(!region_classification_model.empty())
        rfh_region.loadClassifier(region_classification_model);
    if(!color_classification_model.empty())
        rfh_color.loadClassifier(color_classification_model);

}

/// Destructor
FireTrackerSA::~FireTrackerSA(void)
{
}

/// This is a helper method used to generate region training data
/// As it is a task that is very hard to do manually we created it so that you can easilly validate the contours against the groud truth
/// appending the class attribute and returning a string with good and bad contours
/// By good contours we assume the ones where the similarity is greater than 0.8.
/// Bad bad contours we assum the ones that do not have any intersection with the ground truth
std::vector<std::vector<float>> FireTrackerSA::generateRegionTrainingData(std::function<double(std::vector<cv::Point> &dt, cv::Rect &gt)> simFcn, std::vector<cv::Rect> groundTruth){
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
			if(simFcn(current_contours[i], r) > .1){//is out of the gt
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
