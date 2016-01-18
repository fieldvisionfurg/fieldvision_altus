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

// Master include file
#include "VisionImplementationCv.h"

namespace Viscv {

DatasetFDDB::DatasetFDDB(const std::string baseDir,const std::string foldFileName)
    : m_dir(baseDir)
    , m_folds()
{
    m_folds.push_back(foldFileName);
    extractData();
}


DatasetFDDB::~DatasetFDDB(void)
{
}


/// Retorna um vetor com os identificadores de todas as amostras contidas no dataset.
std::vector<std::string> DatasetFDDB::getSamplesID() const
{
    return m_samplesID;
}

/// Retorna um ponteiro para a imagem de uma amostra.
std::shared_ptr<cv::Mat> DatasetFDDB::getImage(const std::string& sampleID) const
{
    std::string sampleFile;
    sampleFile=m_dir+sampleID+".jpg";
    cv::Mat* img = new cv::Mat(cv::imread(sampleFile));
    return std::shared_ptr<cv::Mat>(img);
}

/// Retorna uma lista com os objetos contidos na imagem de uma amostra
std::vector<cv::Rect> DatasetFDDB::getObjects(const std::string& sampleID) const
{
    return m_objects.at(sampleID);
}


/// Executa uma função para cada amostra do dataset.
void DatasetFDDB::forEachSample(std::function<void(cv::Mat&,std::vector<cv::Rect>&,const std::string&)> funct)
{
    for(const std::string& sampleID : m_samplesID){
        assert(sampleID.length()>1);
        // Load sample image
        std::string sampleFile=m_dir+sampleID+".jpg";
        cv::Mat img = cv::imread(sampleFile);
        // Call function passing the sample data
        assert(img.rows > 0 && img.cols > 0);
        if(!img.empty()){

            funct(img,m_objects.at(sampleID),sampleID);
        }
    }
}

void DatasetFDDB::extractData()
{
    m_samplesID.clear();
    m_objects.clear();
    for(const std::string& foldFileName : m_folds){
        // Opens fold file
        std::ifstream ifs (foldFileName, std::ifstream::in);

        // While has not reached end of file
        while(ifs.good()){
            // Read sample ID
            std::string sampID;
            ifs>>sampID;
            if(sampID.length() > 1){
                m_samplesID.push_back(sampID);

                // Create a vector of faces in the image
                int numOfFaces;
                ifs>>numOfFaces;
                std::vector<cv::Rect> faces;
                for(int i=0;i<numOfFaces;i++){
                    // Convert elipse to rectangle
                    double elipseX,elipseY,elipseTheta,elipseXRadious,elipseYRadious,confidence;
                    ifs>>elipseYRadious;
                    ifs>>elipseXRadious;
                    ifs>>elipseTheta;
                    ifs>>elipseX;
                    ifs>>elipseY;
                    ifs>>confidence;

                    double rectX,rectY,rectWidth,rectHeight;
                    rectX=elipseX-elipseXRadious;
                    rectY=elipseY-elipseYRadious;
                    rectWidth=2*elipseXRadious;
                    rectHeight=2*elipseYRadious;

                    faces.push_back(cv::Rect(static_cast<int>(rectX),
                                             static_cast<int>(rectY),
                                             static_cast<int>(rectWidth),
                                             static_cast<int>(rectHeight))
                                    );
                }
                std::pair<std::string,std::vector<cv::Rect>> p(sampID,faces);
                m_objects.insert(p);
            }
        }
    }
}
}
