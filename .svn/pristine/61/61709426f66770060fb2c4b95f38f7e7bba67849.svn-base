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

#ifndef _DATASETFIRE_H
#define _DATASETFIRE_H

#include "FrameAnnotation.h"
#include "VideoAnnotation.h"
#include "VisionCore.h"
#include "VisionImplementationCv.h"


using namespace VisionCore;
using namespace Viscv;
/**
 * The fire dataset is a set of annotated videos. The annotations are in a rectangle shape.
 * One frame can contain 0 or more annotated regions.
*/
class DatasetFire :
    public TrackingDataset<cv::Mat,std::vector<cv::Rect>>
{
public:
    /// Construtor.
    /** É necessário informar o diretório onde está o dataset e o path completo do arquivo
    texto conténdo a lista das imagens e anotações de cada imagem (fornecido junto com o dataset).
    Exemplo:

    */
    DatasetFire(const std::string baseDir);

    /// Destrutor.
    virtual ~DatasetFire(void);

	
    /// Retorna um vetor com os identificadores de todas as amostras contidas no dataset.
    virtual std::vector<std::string> getSamplesID() const;
    /// Retorna um ponteiro para a imagem de uma amostra.
    virtual FrameServer<cv::Mat> *getFrameServer(const std::string& sampleID) const;
    /// Retorna uma lista com os objetos contidos na imagem de uma amostra
    virtual std::vector<std::vector<cv::Rect>> getObjects(const std::string& sampleID) const;
	

private:

    ///  String do diretório do dataset.
    const std::string m_dir;
};

#endif // ndef _DATASETFIRE_H
