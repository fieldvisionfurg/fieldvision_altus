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

#include "TemporalDataExtractor.h"

void TemporalDataExtractor::initialize(int x,int y,int c){
	f_x = x; f_y = y; f_c = c;
	for(int i=0; i < f_c; i++){
		ch_mean.push_back(cv::Mat(f_y, f_x, CV_32F)); ch_std_dev.push_back(cv::Mat(f_y, f_x, CV_32F)); ch_variance.push_back(cv::Mat(f_y, f_x, CV_32F));
	}
	//generate the values
	chn_hist.resize(f_c);

}

void TemporalDataExtractor::addNewFrame(cv::Mat frame){
	if(f_x == 0){initialize(frame.cols, frame.rows, frame.channels());}
	std::vector<cv::Mat> channels;
	cv::split(frame, channels); 
#pragma loop(hint_parallel(0))
	for(int c = 0; c < f_c; c++){
		//for each row in the image
		channels[c].convertTo(channels[c], CV_32F);
		chn_hist[c].push_back(channels[c]);
		//control the history size
		if(chn_hist[c].size() > historySize)
			chn_hist[c].erase(chn_hist[c].begin()+1);
	}
}

void TemporalDataExtractor::computePixelMean(){

	int hist = chn_hist[0].size();
#pragma loop(hint_parallel(0))
	for(int c = 0; c < f_c; c++){
		ch_mean[c] = cv::Mat::zeros(f_y, f_x, CV_32F);
		for(int h = 0; h < hist; h++){
			ch_mean[c]+= chn_hist[c][h];
		}
		ch_mean[c] = ch_mean[c]/hist;
	}
	if(show_dbg_info){
		std::vector<cv::Mat> s;  s.resize(f_c);
		for(int c = 0; c < f_c; c++){
		ch_mean[c].convertTo(s[c], CV_8UC1);
		cv::imshow("mean"+std::to_string(c), s[c]);
		}
	}
}

void TemporalDataExtractor::computePixelStdDev(){

	int hist = chn_hist[0].size();
	cv::Mat diff;
#pragma loop(hint_parallel(0))
	for(int c = 0; c < f_c; c++){
		ch_variance[c] = cv::Mat::zeros(f_y, f_x, CV_32F);
		for(int h = 0; h < hist; h++){
			cv::absdiff(ch_mean[c], chn_hist[c][h], diff);
			ch_variance[c] += diff;
		}
		//compute the standart deviation
#pragma loop(hint_parallel(0))
		for( int y = 0; y < ch_variance[c].rows; y++ )
		{
			float* ptr_var = ch_variance[c].ptr<float>(y);
			float* ptr_std_dev = ch_std_dev[c].ptr<float>(y);
			//for each column in the image
#pragma loop(hint_parallel(0))
			for( int x = 0; x < ch_variance[c].cols; x++ )
			{
				ptr_std_dev[x] = sqrt(ptr_var[x]);
			}
		}
	}

	if(show_dbg_info){
		std::vector<cv::Mat> v, s; v.resize(f_c); s.resize(f_c);
		for(int c = 0; c < f_c; c++){
		ch_variance[c].convertTo(v[c], CV_8UC1);
		ch_std_dev[c].convertTo(s[c], CV_8UC1);
		cv::imshow("variance "+ std::to_string(c), v[c]);
		cv::imshow("std_dev"+ std::to_string(c), s[c]);
		}
	}
}


//Convert a vector to array
cv::Mat TemporalDataExtractor::vector2Mat(std::vector<float> &v, int x, int y){
	cv::Mat out = cv::Mat::zeros(x,y,CV_32F);
	
	for( int yi = 0; yi < out.rows; yi++ )
	{
		float *p = out.ptr<float>(yi);
		//for each column in the image
		int row_strt = y*yi;

#pragma loop(hint_parallel(0))
		for( int xi = 0; xi < out.cols; xi++ )
		{
			//adiciona um valor nos dados do píxel
			p[xi] = v[row_strt + xi];
		}
	}
	return out;
}

std::vector<std::vector<float>> TemporalDataExtractor::mat2vector(cv::Mat &m){
	std::vector<cv::Mat> channels;
	std::vector<std::vector<float>> out;
	out.resize(m.channels());
	cv::split(m, channels); 
	
#pragma loop(hint_parallel(0))
    for(unsigned c = 0; c < channels.size(); c++){
		out[c].resize(m.rows * m.cols);
		for( int y = 0; y < m.rows; y++ )
		{
			float *p = channels[c].ptr<float>(y);
			//for each column in the image
			int row_start = m.cols * y;
			#pragma loop(hint_parallel(0))
			for( int x = 0; x < m.cols; x++ )
			{
				out[c][row_start + x] = p[x];
			}
		}
	}
	return out;
}

//constructor
TemporalDataExtractor::TemporalDataExtractor(void)
{
	f_x= 0; f_y = 0; f_c = 0;
	historySize = 10;
	show_dbg_info = false;
}


TemporalDataExtractor::~TemporalDataExtractor(void)
{
}
