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

#include "VisionImplementationCv.h"

namespace Viscv {

ObjectDetectorFMCv::ObjectDetectorFMCv()
	: objectKeypoints()
	, objectDescriptors()
{
	featDetPtr=new cv::SIFT();
	extractor= new cv::SIFT();

	minNumberOfFeaturesMatching=8;
}


ObjectDetectorFMCv::~ObjectDetectorFMCv()
{
	delete featDetPtr;
	delete extractor;

}

void ObjectDetectorFMCv::setTargetImage(const cv::Mat &img){
	objImg=img.clone();
	featDetPtr->detect(objImg, objectKeypoints);
	extractor->compute(objImg, objectKeypoints, objectDescriptors);
}

std::vector<cv::Rect> ObjectDetectorFMCv::detect(const cv::Mat &img) const{
	// Código baseado em: https://code.google.com/p/find-object/source/browse/trunk/find_object/example/main.cpp
	
	
    std::vector<cv::Rect> foundObjects;
	//Se imagem alvo não foi definida, retorna lista vazia
	if(objImg.empty())
		return foundObjects;

	
	std::vector<cv::KeyPoint> sceneKeypoints;
	
	cv::Mat sceneDescriptors;

	// EXTRACT KEYPOINTS
	featDetPtr->detect(img, sceneKeypoints);

	// EXTRACT DESCRIPTORS
	extractor->compute(img, sceneKeypoints, sceneDescriptors);

    // NEAREST NEIGHBOR MATCHING USING FLANN LIBRARY (included in OpenCV)
    ////////////////////////////
    cv::Mat results;
    cv::Mat dists;
    std::vector<std::vector<cv::DMatch> > matches;
    bool isBinaryDescriptors;
    int k=2; // find the 2 nearest neighbors
    bool useBFMatcher = false; // SET TO TRUE TO USE BRUTE FORCE MATCHER (may give better results with binary descriptors)
    if(objectDescriptors.type()==CV_8U)  {
        // Binary descriptors detected (from ORB, Brief, BRISK, FREAK)
        isBinaryDescriptors = true;
        if(useBFMatcher)  {
            cv::BFMatcher matcher(cv::NORM_HAMMING);
            matcher.knnMatch(objectDescriptors, sceneDescriptors, matches, k);
        }
        else {
            // Create Flann LSH index
            cv::flann::Index flannIndex(sceneDescriptors, cv::flann::LshIndexParams(12, 20, 2), cvflann::FLANN_DIST_HAMMING);
            results = cv::Mat(objectDescriptors.rows, k, CV_32SC1); // Results index
            dists = cv::Mat(objectDescriptors.rows, k, CV_32FC1); // Distance results are CV_32FC1 ?!?!? NOTE OpenCV doc is not clear about that...

            // search (nearest neighbor)
            flannIndex.knnSearch(objectDescriptors, results, dists, k, cv::flann::SearchParams() );
        }
    }
    else {
        // assume it is CV_32F
        isBinaryDescriptors = false;
        if(useBFMatcher) {
            cv::BFMatcher matcher(cv::NORM_L2);
            matcher.knnMatch(objectDescriptors, sceneDescriptors, matches, k);
        }
        else {
            // Create Flann KDTree index
            cv::flann::Index flannIndex(sceneDescriptors, cv::flann::KDTreeIndexParams(), cvflann::FLANN_DIST_EUCLIDEAN);
            results = cv::Mat(objectDescriptors.rows, k, CV_32SC1); // Results index
            dists = cv::Mat(objectDescriptors.rows, k, CV_32FC1); // Distance results are CV_32FC1

            // search (nearest neighbor)
            flannIndex.knnSearch(objectDescriptors, results, dists, k, cv::flann::SearchParams() );
        }
    }


    ////////////////////////////
    // PROCESS NEAREST NEIGHBOR RESULTS
    ////////////////////////////

    // Find correspondences by NNDR (Nearest Neighbor Distance Ratio)
    float nndrRatio = 0.6f;
    std::vector<cv::Point2f> mpts_1, mpts_2; // Used for homography
    std::vector<int> indexes_1, indexes_2; // Used for homography
    std::vector<uchar> outlier_mask;  // Used for homography
    // Check if this descriptor matches with those of the objects
    if(!useBFMatcher)
    {
        for(int i=0; i<objectDescriptors.rows; ++i)
        {
			// Apply NNDR
			//printf("q=%d dist1=%f dist2=%f\n", i, dists.at<float>(i,0), dists.at<float>(i,1));
			if(isBinaryDescriptors || //Binary, just take the nearest
				dists.at<float>(i,0) <= nndrRatio * dists.at<float>(i,1))
			{
				mpts_1.push_back(objectKeypoints.at(i).pt);
				indexes_1.push_back(i);

				mpts_2.push_back(sceneKeypoints.at(results.at<int>(i,0)).pt);
				indexes_2.push_back(results.at<int>(i,0));
			}
        }
    }
    else
    {
        for(unsigned int i=0; i<matches.size(); ++i)
        {
            // Apply NNDR
            //printf("q=%d dist1=%f dist2=%f\n", matches.at(i).at(0).queryIdx, matches.at(i).at(0).distance, matches.at(i).at(1).distance);
            if(isBinaryDescriptors || //Binary, just take the nearest
                matches.at(i).at(0).distance <= nndrRatio * matches.at(i).at(1).distance)
            {
                mpts_1.push_back(objectKeypoints.at(matches.at(i).at(0).queryIdx).pt);
                indexes_1.push_back(matches.at(i).at(0).queryIdx);

                mpts_2.push_back(sceneKeypoints.at(matches.at(i).at(0).trainIdx).pt);
                indexes_2.push_back(matches.at(i).at(0).trainIdx);
            }
        }
    }

    // FIND HOMOGRAPHY
    if(static_cast<int>(mpts_1.size()) >= minNumberOfFeaturesMatching)
    {
        cv::Mat H = findHomography(mpts_1,
                        mpts_2,
                        cv::RANSAC,
                        1.0,
                        outlier_mask);

		// Transforma contorno do objeto usando homografia
		std::vector<cv::Point2f> srcPoints;
		srcPoints.push_back(cv::Point2f(0.0f,0.0f));
        srcPoints.push_back(cv::Point2f(static_cast<float>(objImg.cols-1),0.0f));
        srcPoints.push_back(cv::Point2f(static_cast<float>(objImg.cols-1),static_cast<float>(objImg.rows-1)));
        srcPoints.push_back(cv::Point2f(0.0f,static_cast<float>(objImg.rows-1)));

		std::vector<cv::Point2f> dstPoints;
		cv::perspectiveTransform(srcPoints,dstPoints,H);

		cv::Rect r = cv::boundingRect(dstPoints);
		foundObjects.push_back(r);

    }
    else
    {
            printf("Not enough matches (%d) for homography...\n", (int)mpts_1.size());
    }

	return foundObjects;
}

}
