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

#ifndef _DATASETFDDB_H
#define _DATASETFDDB_H

#include "VisionImplementationCv.h"

namespace Viscv
{

/// Dataset público de várias imagens contendo pessoas em ambientes do dia-a-dia. 
/** Utilizado principalmente para detecção de faces. Para cada imagem contida no dataset
existe uma anotação (extraidas de um arquivo texto) que indica a localização das faces.

\ingroup Viscv
*/
class DatasetFDDB :
    public VisionCore::DetectionDataset<cv::Mat,cv::Rect>
{
public:
	/// Construtor.
	/** É necessário informar o diretório onde está o dataset e o path completo do arquivo 
	texto conténdo a lista das imagens e anotações de cada imagem (fornecido junto com o dataset).
	Exemplo:
	DatasetFDDB db("E:/FURG/Datasets/FDDB/","E:/FURG/Datasets/FDDB-folds/FDDB-fold-01-ellipseList.txt");
	*/
	DatasetFDDB(const std::string baseDir,const std::string fold);

	/// Destrutor.
	virtual ~DatasetFDDB(void);

	
	/// Retorna um vetor com os identificadores de todas as amostras contidas no dataset.
	virtual std::vector<std::string> getSamplesID() const;
    /// Retorna um ponteiro para a imagem de uma amostra.
	virtual std::shared_ptr<cv::Mat> getImage(const std::string& sampleID) const;
	/// Retorna uma lista com os objetos contidos na imagem de uma amostra
	virtual std::vector<cv::Rect> getObjects(const std::string& sampleID) const;
	

	/// Executa uma função para cada amostra do dataset.
	/** Esta função abre cada uma das imagens do dataset, extrai as informações de anotações (locais das faces),
	e chama a função passada como argumento. O sampleId de cada amostra é o nome do arquivo da imagem.
	*/
    virtual void forEachSample(std::function<void(cv::Mat&,std::vector<cv::Rect>&,const std::string&)> funct);

private:
	///  String do diretório do dataset.
	const std::string m_dir;

	/// Lista de strings com os "folds" conténdo as informações (nomes dos arquivos e anotações) das amostras
	std::vector<std::string> m_folds;

    /// Vector containing all samplesID's in the dataset.
    std::vector<std::string> m_samplesID;

    /// Stores the objects in samples
    std::map<std::string,std::vector<cv::Rect>> m_objects;

    /// Extract samplesID's and objects from folds
    void extractData();
};

}

#endif // ndef _DATASETFDDB_H
