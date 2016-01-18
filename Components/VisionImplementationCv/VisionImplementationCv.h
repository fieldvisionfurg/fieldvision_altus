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

/**
* \file VisionImplementationCv.h
* \brief This file defines the Viscv component, which implements high-level computer vision methods using OpenCv library.
* \see Viscv
*
* \defgroup Viscv Viscv
* \brief This component groups some detectors and trackers based on OpenCv.
*
* This component provides some detection and tracking methods (such as Haar Cascade Classifier detectors, Lucas-Kanade
* tracker, SIFT based detection, etc) based on OpenCv library.
* The methods can be usefull in applications, as well as serve as examples on how to implement CvWorks interfaces.
*
*\see VisionImplementationCv.h
*
*/

#ifndef _VISIONIMPLEMENTATIONCV_H
#define _VISIONIMPLEMENTATIONCV_H

#include <cstdlib>
#include <stdio.h>
#include <math.h>
#include <algorithm>
#include <vector>
#include <random>
#include <list>
#include <iostream>
#include <fstream>
#include <string>
#include <thread>
#include <functional>
#include <assert.h>

//Eigen (matrix library)
#include <Eigen/Core>
#include <Eigen/Dense>
#include <Eigen/StdVector>

//OpenCv
#include "opencv2/core/core.hpp" 
#include "opencv2/highgui/highgui.hpp" 
#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/video/tracking.hpp"
#include "opencv2\video\background_segm.hpp"
#include "opencv2/calib3d/calib3d.hpp"
#include "opencv2/contrib/contrib.hpp"
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/nonfree/features2d.hpp>
#include <opencv2/ml/ml.hpp>
#include <assert.h>
#include <memory>

//CvWorks core
#include "VisionCore.h"
//Overload usefull CvWorks functions
#include "VisionHelperCv.h"

//Viscv component.
/// Provides several computer vision methods for detection an.
namespace Viscv {}
#include "CircleDetectorHTCF.h"
#include "CircleTrackerHTCF.h"
#include "ObjectDetectorFMCv.h"
#include "ARTagDetectorBLP.h"
#include "FrameServerCv.h"
#include "ObjectTrackerCCCv.h"
#include "ObjectDetectorCCCv.h"
#include "MultiObjectTrackerCCCv.h"
#include "FeatureTrackerKLTCv.h"
#include "CvImageGUI.h"
#include "ObjectTrackerPFHist.h"
#include "ObjectTrackerKLT.h"
#include "LineDetectorHT.h"
#include "DatasetFDDB.h"
#include "ColorBlobDetectorHF.h"
#include "TemplMatchingDetector.h"
#include "TemplMatchingTracker.h"


#endif // ndef _VISIONIMPLEMENTATIONCV_H
