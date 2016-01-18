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
#include "DatasetFire.h"

#include <QDir>

using namespace VisionCore;
using namespace Viscv;

DatasetFire::DatasetFire(const std::string baseDir)
    : m_dir(baseDir)
{

}


DatasetFire::~DatasetFire(void)
{
}


/// Retorna um vetor com os identificadores de todas as amostras contidas no dataset.
std::vector<std::string> DatasetFire::getSamplesID() const
{
    std::vector<std::string> samplesID;


    QDir d;
    d.cd(QString::fromStdString(m_dir));
    d.setNameFilters(QStringList("*.xml"));
    d.setFilter(QDir::Files | QDir::NoDotAndDotDot | QDir::NoSymLinks);

    QStringList fileList = d.entryList();
    for (int i=0; i<fileList.count(); i++)
    {

        samplesID.push_back(fileList[i].replace(".xml","").toStdString());
        //std::cout << fileList[i].toStdString() << std::endl;

    }

    return samplesID;
}

/// Retorna um video.
FrameServer<cv::Mat> *DatasetFire::getFrameServer(const std::string& sampleID) const
{
    FrameServer<cv::Mat> *fs = new FrameServerCv(m_dir + sampleID + ".mp4");
    return fs;
}

/// Retorna um vetor com os objetos contidos na imagem de uma amostra
std::vector<std::vector<cv::Rect>> DatasetFire::getObjects(const std::string& sampleID) const
{
    std::vector<std::vector<cv::Rect>> objs;
    VideoAnnotation va;
    va.loadFromFile(m_dir + sampleID + ".xml");
    for(unsigned f=0; f < va.frameCount; f++){
        std::vector<cv::Rect> oneFrame;

            for(cv::Rect r: va.getFrameAt(f).rectOnFrame){
                oneFrame.push_back(r);
            }
            objs.push_back(oneFrame);
        }

    return objs;
}



