//// Thats a replacement for the texture.h provided with the OpenCv library.
//// The OpenCv file has some problems and is not updated anymore given that it is contained in the legacy module.
//// Calculation of a texture descriptors from GLCM (Grey Level Co-occurrence Matrix'es)
//// For further information check Haralick (1973).
//// Original Source: http://www.cnblogs.com/xiangshancuizhu/archive/2011/07/15/2107656.html

#pragma once
#include "opencv/cv.h"

#ifndef TextureGLCM_H
#define TextureGLCM_H


#define CV_GLCM_OPTIMIZATION_NONE                   -2
#define CV_GLCM_OPTIMIZATION_LUT                    -1
#define CV_GLCM_OPTIMIZATION_HISTOGRAM              0

#define CV_GLCMDESC_OPTIMIZATION_ALLOWDOUBLENEST    10
#define CV_GLCMDESC_OPTIMIZATION_ALLOWTRIPLENEST    11
#define CV_GLCMDESC_OPTIMIZATION_HISTOGRAM          4

#define CV_GLCMDESC_ENTROPY                         0
#define CV_GLCMDESC_ENERGY                          1
#define CV_GLCMDESC_HOMOGENITY                      2
#define CV_GLCMDESC_CONTRAST                        3
#define CV_GLCMDESC_CLUSTERTENDENCY                 4
#define CV_GLCMDESC_CLUSTERSHADE                    5
#define CV_GLCMDESC_CORRELATION                     6
#define CV_GLCMDESC_CORRELATIONINFO1                7
#define CV_GLCMDESC_CORRELATIONINFO2                8
#define CV_GLCMDESC_MAXIMUMPROBABILITY              9

#define CV_GLCM_ALL                                 0
#define CV_GLCM_GLCM                                1
#define CV_GLCM_DESC                                2

#define CV_MAX_NUM_GREY_LEVELS_8U 256
class TextureGLCM
{
public:
    //Struct
    struct GLCM
    {
        int matrixSideLength;
        int numMatrices;
        double*** matrices;
 
        int numLookupTableElements;
        int forwardLookupTable[CV_MAX_NUM_GREY_LEVELS_8U];
        int reverseLookupTable[CV_MAX_NUM_GREY_LEVELS_8U];
 
        double** descriptors;
        int numDescriptors;
        int descriptorOptimizationType;
        int optimizationType;
    };
    TextureGLCM(void);
    ~TextureGLCM(void);
    /* srcStepDirections should be static array..or if not the user should handle de-allocation */
 
    GLCM* CreateGLCM( const IplImage* srcImage, int stepMagnitude, const int* srcStepDirections, int numStepDirections, int optimizationType );
    void CreateGLCMDescriptors( GLCM* destGLCM, int descriptorOptimizationType);
    double GetGLCMDescriptor( GLCM* GLCM, int step, int descriptor );
    void GetGLCMDescriptorStatistics( GLCM* GLCM, int descriptor, double*_average, double* _standardDeviation );
    IplImage* CreateGLCMImage( GLCM* GLCM, int step );
    void CreateGLCM_LookupTable_8u_C1R( const uchar* srcImageData, int srcImageStep, CvSize srcImageSize, GLCM* destGLCM, int* steps, int numSteps, int* memorySteps );
    void CreateGLCMDescriptors_AllowDoubleNest( GLCM* destGLCM, int matrixIndex );
    void ReleaseGLCM( GLCM** GLCM, int flag );
 
};
#pragma once

#endif