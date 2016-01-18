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

#include "C45Helper.h"

//evaluate the classifier using a ground thru csv file
std::string C45Helper::evaluateClassifier(const std::string file_name, char delimiter, bool ignore_first_row, bool ignore_first_column){
	std::vector<std::vector<std::string>> data_vector;
	readCsvFile(file_name, delimiter, data_vector);
	int attributes_per_sample = ignore_first_column? data_vector.at(1).size() -2 : data_vector.at(1).size() - 1;
	int number_of_test_samples = ignore_first_row? data_vector.size() - 1 : data_vector.size();
	cv::Mat test_data = cv::Mat(number_of_test_samples, attributes_per_sample, CV_32FC1);
	cv::Mat test_classes  = cv::Mat(number_of_test_samples, 1, CV_32FC1);
	int first_col = ignore_first_column? 1 : 0;
	int first_row = ignore_first_row? 1 : 0;

	//convert the vector to a mat 
	for(unsigned i = first_row; i < data_vector.size(); i++){
		for(unsigned j = first_col; j < data_vector.at(first_row).size() -1; j++){
            test_data.at<float>(i-first_row, j-first_col) = (float)str2double(data_vector[i][j]);
		}
        test_classes.at<float>(i-first_row,0) = (float)str2double(data_vector[i][data_vector[i].size()-1]);
	}

	std::string report = "";
	// perform classifier testing and report results
	cv::Mat test_sample;
	int correct_class = 0;
	int wrong_class = 0;
	std::vector<int>false_positives; for (int i=0; i < number_of_classes; i++){false_positives.push_back(0);}
	double result; // value returned from a prediction

	for (int tsample = 0; tsample < number_of_test_samples; tsample++)
	{

		// extract a row from the testing matrix
		cv::Mat test_sample = test_data.row(tsample);

		// run random forest prediction
		result = classify(test_sample);
		// if the prediction and the (true) testing classification are the same
		// (N.B. openCV uses a floating point decision tree implementation!)
		if (fabs(result - test_classes.at<float>(tsample, 0))
			>= FLT_EPSILON)
		{
			// if they differ more than floating point error => wrong class
			wrong_class++;
			false_positives[(int) result]++;

		}
		else
		{
			// otherwise correct
			correct_class++;
		}
	}
	report += "\nResults on the testing database:" + file_name;
	report += "\n\tCorrect classification: " + std::to_string(correct_class) + "\t" + std::to_string((double) correct_class*100/test_classes.rows) + "%";
	report += "\n\tWrong classifications: " + std::to_string(wrong_class) + "\t" + std::to_string((double) wrong_class*100/test_classes.rows)+"%";

	for (int i = 0; i < number_of_classes; i++)
	{
		report += "\n\tClass ("+ std::to_string(i)+") false postives "+ std::to_string(false_positives[i]) + "\t" + std::to_string((double) false_positives[i]*100/test_classes.rows)+ "%";
	}

	return report;
}

//predict
double C45Helper::classify(cv::Mat &test_sample){
	return rtree->predict(test_sample)->value;
}

//set the params and train
int C45Helper::trainClassifier(){
	rtree->train(training_data, CV_ROW_SAMPLE, training_classes,
		cv::Mat(), cv::Mat(), var_type, cv::Mat(), params);
	return 1;
}

	//Save the classifier to file
void C45Helper::saveClassifier(const std::string file_name){
	rtree->save(file_name.c_str());
}

	//Load a classifier from file
void C45Helper::loadClassifier(const std::string file_name){
	rtree->load(file_name.c_str());
}

// The readDataFromCsv funcion prepares the training dataset by reading a csv file
// The public variables training_data, training_classes, var_type are prepared 
// to be used in a supervised learning method such as SVM, C4.5 or random forests
// We assume that the last column in the csv file is the class and the other ones are numeric.
// If that is not your case you can edit the var_type variable so that it fits your problem input data.
// Set ignore_first_row to true if your data has a header.
// Set ignore_first_column to true if it is a identifier and it should not be used on the classification.
int C45Helper::readTrainDataFromCsv(const std::string file_name, char delimiter,  bool ignore_first_row, bool ignore_first_column){
	std::vector<std::vector<std::string>> data_vector;
	readCsvFile(file_name, delimiter, data_vector);
	int attributes_per_sample = ignore_first_column? data_vector.at(1).size() -2 : data_vector.at(1).size() - 1;
	int number_of_training_samples = ignore_first_row? data_vector.size() - 1 : data_vector.size();
	training_data = cv::Mat(number_of_training_samples, attributes_per_sample, CV_32FC1);
	training_classes  = cv::Mat(number_of_training_samples, 1, CV_32FC1);
	//cout << training_data.size() << endl; cout << training_classes.size() << endl;
	int first_col = ignore_first_column? 1 : 0;
	int first_row = ignore_first_row? 1 : 0;

	//convert the vector to a mat 
	for(unsigned i = first_row; i < data_vector.size(); i++){
		for(unsigned j = first_col; j < data_vector[first_row].size() -1; j++){
            training_data.at<float>(i-first_row, j-first_col) = (float)str2double(data_vector[i][j]);
		}
        training_classes.at<float>(i-first_row,0) = (float)str2double(data_vector[i][data_vector[i].size()-1]);
	}

	//cout << training_classes;
	// define all the attributes as numerical
	// alternatives are CV_VAR_CATEGORICAL or CV_VAR_ORDERED(=CV_VAR_NUMERICAL)
	// that can be assigned on a per attribute basis
	// this is a classification problem (i.e. predict a discrete number of class
	// outputs) so reset the last (+1) output var_type element to CV_VAR_CATEGORICAL
	var_type = cv::Mat(attributes_per_sample + 1, 1, CV_8U );
	var_type.setTo(cv::Scalar(CV_VAR_NUMERICAL) ); // all inputs are numerical
	var_type.at<uchar>(attributes_per_sample, 0) = CV_VAR_CATEGORICAL;

	return 1;
}


//Read the CSV file
void C45Helper::readCsvFile(std::string file_name, char delimiter, std::vector<std::vector<std::string>> &data){
	std::vector<std::string> row;
	std::string line;
	std::ifstream in(file_name);
	if (in.fail())  { std::cout << "File not found" << std::endl; return; }

	while(getline(in, line)  && in.good() )
	{
		csvlinePopulate(row, line, delimiter);
		data.push_back(row);
	}
	in.close();

}

// Registra os valores de cada ponto presente no arquivo CSV ( cada linha da base de dados)
void C45Helper::csvlinePopulate(std::vector<std::string> &record, const std::string& line, char delimiter)
{
	int linepos=0;
	int inquotes=false;
	char c;
	int linemax=line.length();
	std::string curstring;
	record.clear();
	record.reserve(5);
	while(line[linepos]!=0 && linepos < linemax)
	{

		c = line[linepos];
		if (c != '"' && c != delimiter && c != '\r' && c != '\n'){
			curstring.push_back(c);
		}else{

			if (!inquotes && curstring.length()==0 && c=='"')
			{
				//beginquotechar
				inquotes=true;
			}
			else if (inquotes && c=='"')
			{
				//quotechar
				if ( (linepos+1 <linemax) && (line[linepos+1]=='"') )
				{
					//encountered 2 double quotes in a row (resolves to 1 double quote)
					curstring.push_back(c);
					linepos++;
				}
				else
				{
					//endquotechar
					inquotes=false;
				}
			}
			else if (!inquotes && c==delimiter)
			{
				//end of field
				record.push_back( curstring );
				curstring="";
			}
			else if (!inquotes && (c=='\r' || c=='\n') )
			{
				if(curstring.length()>0)
					record.push_back( curstring );
				return;
			}
			else
			{
				curstring.push_back(c);
			}
		}

		linepos++;
	}
	record.push_back( curstring );
	return;
}

//CSV importing auxiliary functions
//Converts the input string to double
double C45Helper::str2double(std::string str)
{
	const char* strc = str.c_str();
	char* endptr;
	double value = strtod(strc, &endptr);
	if (*endptr){//error
		//categoric classes are converted to numeric
		if(str.find("t") != std::string::npos || str.find("y") != std::string::npos|| str.find("s")!= std::string::npos){
			return 1;
		}else{
			return 0;
		}
	}
	return value;
}

// Can the string be converted to number
bool C45Helper::is_number(std::string str){
	const char* strc = str.c_str();
	char* endptr;
	double value = strtod(strc, &endptr);
	if (*endptr){
		//error
		return false;
	}
    value = 0;
	return true;
}

//Constructor
C45Helper::C45Helper(int number_of_classes)
{
	rtree = new CvDTree;
	this->number_of_classes = number_of_classes;
	//set the a priory values of each class
	for(int i = 0; i < number_of_classes; i++)
		priors.push_back(1);

	params = CvDTreeParams( 9, // max depth
		50, // min sample count
		0, // regression accuracy: N/A here
		false, // compute surrogate split, as we have missing data
		2, // max number of categories (use sub-optimal algorithm for larger numbers)
		10, // the number of cross-validation folds
		true, // use 1SE rule => smaller tree
		true, // throw away the pruned tree branches
		priors.data() // the array of priors, the bigger p_weight, the more attention the class gets
		);


}


C45Helper::~C45Helper(void)
{
}
