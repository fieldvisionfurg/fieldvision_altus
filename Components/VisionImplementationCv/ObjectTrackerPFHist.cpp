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

/*
Default constructor method.
*/
ObjectTrackerPFHist::ObjectTrackerPFHist(const unsigned int numParticles) 
    : AbstractParticleFilteringTracker<cv::Mat,cv::Rect>(cv::Rect(0,0,10,10), numParticles)
    , m_choleskyTransMat()
    , m_transitionMatrix()
	, m_blueHist()
	, m_greenHist()
	, m_redHist()
{
	m_lostTrack=true;
    m_transitionMatrix<<20,0,0,0,20,0,0,0,4;
        
    //CHOLESKY DECOMPOSITION OF TRANSITION MATRIX
	Eigen::LLT<Eigen::Matrix3d> cholesky(3); 
	cholesky.compute(m_transitionMatrix);
	m_choleskyTransMat=cholesky.matrixL();
}

/*
Constructor method.
*/
ObjectTrackerPFHist::ObjectTrackerPFHist(const cv::Mat& initialImg, const cv::Rect& initialObj,
                                         const unsigned int numParticles) 
    : AbstractParticleFilteringTracker<cv::Mat,cv::Rect>(initialObj, numParticles)
    , m_choleskyTransMat()
    , m_transitionMatrix()
{
	computeHistogram(initialImg(initialObj),m_redHist,m_greenHist,m_blueHist);

    m_transitionMatrix<<20,0,0,0,20,0,0,0,4;
        
    //CHOLESKY DECOMPOSITION OF TRANSITION MATRIX
	Eigen::LLT<Eigen::Matrix3d> cholesky(3); 
	cholesky.compute(m_transitionMatrix);
	m_choleskyTransMat=cholesky.matrixL();
}


/*
Destructor method.
*/
ObjectTrackerPFHist::~ObjectTrackerPFHist()
{
}

void ObjectTrackerPFHist::reset(const cv::Mat& img,const cv::Rect& rectangle)
{
	computeHistogram(img(rectangle),m_redHist,m_greenHist,m_blueHist);
	VisionCore::AbstractParticleFilteringTracker<cv::Mat,cv::Rect>* p = this;
	p->reset(rectangle);
}

cv::Rect ObjectTrackerPFHist::averageObj(const std::vector<cv::Rect>& objects,
                                         const std::vector<double>& weights)
{

    double sumX=0.0;
    double sumY=0.0;
    double sumH=0.0;
    double sumW=0.0;
	double sumWeights=0.0;
    for(unsigned int i=0;i<objects.size();i++){
        sumX+=weights[i]*objects[i].x;
        sumY+=weights[i]*objects[i].y;
        sumH+=weights[i]*objects[i].height;
        sumW+=weights[i]*objects[i].width;
		sumWeights+=weights[i];
    }
    sumX/=sumWeights;
    sumY/=sumWeights;
    sumH/=sumWeights;
    sumW/=sumWeights;
    return cv::Rect((int)sumX,(int)sumY,(int)sumW,(int)sumH);
}


double ObjectTrackerPFHist::evalObj(const cv::Mat& img, const cv::Rect& obj)
{
	double m=0.0;
	if(obj.height>0 &&  obj.width>0 ){
		cv::Mat redHist;
		cv::Mat greenHist;
		cv::Mat blueHist;
		computeHistogram(img(obj),redHist,greenHist,blueHist);
		m = matchHistogram(redHist,greenHist,blueHist);
	}
	return m;
}


void ObjectTrackerPFHist::sampleTransition(const cv::Rect& oldObj,
                                           cv::Rect& newObj)
{
    //SAMPLE FROM A GAUSSIAN CENTERED AT oldObj
    using namespace std;
    const unsigned int xDim = 3;
	const Eigen::Vector3d mean(oldObj.x,oldObj.y,oldObj.height); 

	
	//GENERATES A ZERO MEAN UNIT VARIANCE SAMPLE
	Eigen::Vector3d sample;
	normal_distribution<double> normRnd(0.0,1.0);
	//variate_generator<mt19937, normal_distribution<>>  r(this->m_randEngine, normal_distribution<>(0.0, 1.0));
	for(unsigned int i=0;i<xDim;i++){
		sample(i)=normRnd(this->m_randEngine);
	}
	
	//INTRODUCE VARIANCE AND MEAN
	sample=this->getCholeskyTransMat()*sample+mean;
	newObj.x=(int)sample(0);
	newObj.y=(int)sample(1);
	newObj.height=(int)sample(2);
	newObj.width=newObj.height;
}


void ObjectTrackerPFHist::computeHistogram(const cv::Mat& src,cv::Mat& redHist,cv::Mat& greenHist,cv::Mat& blueHist) {

	/// Baseado no exemplo: http://docs.opencv.org/doc/tutorials/imgproc/histograms/histogram_calculation/histogram_calculation.html

  /// Separate the image in 3 places ( B, G and R )
  std::vector<cv::Mat> bgr_planes;
  split( src, bgr_planes );

  /// Establish the number of bins
  int histSize = 50;

  /// Set the ranges ( for B,G,R) )
  float range[] = { 0, 256 } ;
  const float* histRange = { range };

  bool uniform = true; bool accumulate = false;


  /// Compute the histograms:
  calcHist( &bgr_planes[0], 1, 0, cv::Mat(), blueHist, 1, &histSize, &histRange, uniform, accumulate );
  calcHist( &bgr_planes[1], 1, 0, cv::Mat(), greenHist, 1, &histSize, &histRange, uniform, accumulate );
  calcHist( &bgr_planes[2], 1, 0, cv::Mat(), redHist, 1, &histSize, &histRange, uniform, accumulate );


  /// Needs Normalize?????

}

double ObjectTrackerPFHist::matchHistogram(cv::Mat& redHist,cv::Mat& greenHist,cv::Mat& blueHist) {
	int method = CV_COMP_CORREL;
	double m=0.0;
	m+=cv::compareHist(m_redHist,redHist,method);
	m+=cv::compareHist(m_greenHist,greenHist,method);
	m+=cv::compareHist(m_blueHist,blueHist,method);
	return m;
}

/*
Returns the value of member 'm_choleskyTransMat'.
*/
inline const Eigen::Matrix3d& ObjectTrackerPFHist::getCholeskyTransMat() const
{
    return m_choleskyTransMat;
}

}
