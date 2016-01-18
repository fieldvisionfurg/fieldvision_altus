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
#pragma once
// Master include file
#include "VisionImplementationCv.h"


#ifndef _C45HELPER_H
#define _C45HELPER_H


// The C45Helper is a class prepared to load a '.csv' training file, evaluate using a '.csv' test file and after that use it to classify sample by sample.
// The params are preconfigured to perform well and achieve high accuracy with the most usual datasets. You can check them and uptate them by changing the public variables that start with 'p_'.
// Most of the configuration is done with zero configuration.
// The simplest usage example would be:
//		C45Helper rfh(2); //two is the number of possible classes (here we suppose it is a binary classification) 
//		rfh.readTrainDataFromCsv("train_data.csv", ',', false, false);
//		rfh.trainClassifier();
//		rfh.evaluateClassifier("test_data.csv", ',', false, false);
class C45Helper
{
private:
	CvDTree* rtree;
	//
	int number_of_classes;

	//csv reader
	void csvlinePopulate(std::vector<std::string> &record, const std::string& line, char delimiter);
	void readCsvFile(std::string file_name, char delimiter, std::vector<std::vector<std::string>> &data);
	//csv reader auxiliar funcions
	double str2double(std::string str);
	bool is_number(std::string str);

public:
	// Evaluate the classifier using a ground thru csv file.
	// Loads the test data, runs the classification tree and shows the statistics.
	std::string evaluateClassifier(const std::string file_name, char delimiter = ',', bool ignore_first_row = true , bool ignore_first_column = true);

	//Load train data from csv
	//The basic configuration is done
	//We consider that the class attribute is the last one
	int readTrainDataFromCsv(const std::string file_name, char delimiter = ',', bool ignore_first_row = true , bool ignore_first_column = true);
	
	//Trains the new classifier
	int trainClassifier();

	//Save the classifier to file
	void saveClassifier(const std::string file_name);

	//Load a classifier from file
	void loadClassifier(const std::string file_name);

	//predict the output for one sample
	double classify(cv::Mat &test_sample); 

	//params can be configured directly
  	CvDTreeParams params;
	
	
	// define the parameters for training the random forest (trees)
	// weights of each classification for classes
    // (all equal as equal samples of each digit)
	std::vector<float> priors;

	cv::Mat training_data, training_classes, var_type;
	cv::Mat predicted_classes;

	//Constructor
	C45Helper(int number_of_classes = 2);
	~C45Helper(void);
};

#endif //
