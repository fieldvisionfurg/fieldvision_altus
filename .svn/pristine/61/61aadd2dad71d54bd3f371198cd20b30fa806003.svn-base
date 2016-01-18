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

#ifndef _OBJECTTRACKERPFHIST_H
#define _OBJECTTRACKERPFHIST_H
#include "VisionImplementationCv.h"

namespace Viscv
{

/// Particle Filtering with histogram matching.
/** 

\ingroup Viscv
*/
class ObjectTrackerPFHist
    : public VisionCore::AbstractParticleFilteringTracker<cv::Mat,cv::Rect>
{
private:
    Eigen::Matrix3d m_choleskyTransMat;
    Eigen::Matrix3d m_transitionMatrix;

	cv::Mat m_redHist;
	cv::Mat m_greenHist;
	cv::Mat m_blueHist;

private:
    virtual cv::Rect averageObj(const std::vector<cv::Rect>& objects,
                                const std::vector<double>& weights);
    virtual double evalObj(const cv::Mat& img, const cv::Rect& obj);
    virtual void sampleTransition(const cv::Rect& oldObj, cv::Rect& newObj);
	void computeHistogram(const cv::Mat& src,cv::Mat& redHist,cv::Mat& greenHist,cv::Mat& blueHist);
	double matchHistogram(cv::Mat& redHist,cv::Mat& greenHist,cv::Mat& blueHist);

protected:
    /// Returns the value of member 'm_choleskyTransMat'.
    const Eigen::Matrix3d& getCholeskyTransMat() const;

public:
	/// Default constructor method.
    ObjectTrackerPFHist(const unsigned int numParticles = 100);
    /// Constructor method.
    ObjectTrackerPFHist(const cv::Mat& initialImg, const cv::Rect& initialObj,const unsigned int numParticles = 100);
    /// Destructor method.
    virtual ~ObjectTrackerPFHist();
	/// Reinicia o rastreador para um estado, redefinindo os histogramas.
	void reset(const cv::Mat& img,const cv::Rect& rectangle);


};

}

#endif // ndef _OBJECTTRACKERPFHIST_H
