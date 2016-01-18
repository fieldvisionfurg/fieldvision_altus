/*******************************************************************************\
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

/*******************************************************************************/
/**
* \file VisionEvaluation.h
* \brief Implementation of generic validation algorithms for CvWorks interfaces.
* \see Evaluation
* 
* \defgroup Evaluation Evaluation
* \brief Implementation of generic validation algorithms for CvWorks interfaces.
*
* In order to use this evaluation methods you need a detector (or tracker) and a set of samples with the ground truth.
*
* \ingroup Core
*/


#ifndef _VISIONEVALUATION_H
#define _VISIONEVALUATION_H

#include "../../ExternalLibraries/CsvUtil/HungarianAlgorithm.h"
#include "VisionCore.h"

namespace VisionCore {
/// Evaluation methods for computer vision tasks.
namespace Evaluation {

template<class TImg,typename TObj>
class DetectionSubDataset;


/// Abstract class for a generic detection dataset, contining images and its ground-truth.
/** Each sample in the dataset has an image and a vector of objects considered as ground-truth.
 *
 * Each sample has an unique ID. A vector with all samples IDs can be retrieved by method 'getSamplesID()'.
 *
 * The 'getImage(sampleID)' method returns one image from the dataset and the 'getObjects(sampleID)' method returns the
 * respective ground-truth objects.
 *
 * Derived classes implementing this interface must implement the three methods mentioned above.
 *
 *   \param TImg Image type.
 *   \param TObj Detected object type.
 *   \ingroup Evaluation
*/
template<class TImg,typename TObj>
class DetectionDataset
{
public:
    /// Destructor method.
    virtual ~DetectionDataset();

    /// Returns a vector of IDs for all samples in the dataset.
    /// Each sample should be identified by a unique string ID.
    /// This ID can be anything (a number, the name of file, etc). It is used mainly to retrieve a sample
    /// through function getImage and getObjects.
    virtual std::vector<std::string> getSamplesID() const = 0;

    /// Returns a pointer to one of the images in the dataset.
    /// \param sampleID The unique identifier for the sample you are fetching.
    virtual std::shared_ptr<TImg> getImage(const std::string& sampleID) const = 0;

    /// Returns a vector of objects contained in a sample.
    /// This could be a vector of all the faces that appear on a image when working with a face dataset.
    /// \param sampleID The unique identifier for the sample you are fetching.
    virtual std::vector<TObj> getObjects(const std::string& sampleID) const = 0;

    /// Returns a dataset containing a subset of samples.
    /// The vector samplesID specify the subset samples. If some sampleID does not exist in the original dataset
    /// an exception is throw. The subset mantains a reference to the original dataset, so the subset is only accessible
    /// on the scope of the original dataset.
    DetectionSubDataset<TImg,TObj> getSubset(std::vector<std::string> samplesID);

    /// Splits the dataset in several subdatasets according to proportions (must sum to 1).
    /// \todo: Add option to split samples randomly. Now it splits in order.
    std::vector<DetectionSubDataset<TImg,TObj>> splitDataset(std::vector<double> proportions);

    /// For each sample in dataset execute a function.
    /// \param TImg Image type
    /// \param TObj Object type of the ground truth annotations.
    /// \param funct A function that will be execute for each sample in the dataset.
    virtual void forEachSample(std::function<void(TImg&,std::vector<TObj>&,std::string&)> funct);

    typedef TImg ImgType;
    typedef TObj ObjType;
};


template<class TImg,typename TObj>
void DetectionDataset<TImg,TObj>::forEachSample(std::function<void(TImg&,std::vector<TObj>&,std::string&)> funct)
{
    std::vector<std::string> samplesID =  this->getSamplesID();
    for(std::string& sampID : samplesID)
    {
        std::shared_ptr<TImg> img = this->getImage(sampID);
        std::vector<TObj> obj = this->getObjects(sampID);
        funct(*img,obj,sampID);
    }
}

template<class TImg,typename TObj>
DetectionSubDataset<TImg,TObj> DetectionDataset<TImg,TObj>::getSubset(std::vector<std::string> samplesID)
{
    DetectionSubDataset<TImg,TObj> subset(*this,samplesID);
    return subset;
}

template<class TImg,typename TObj>
std::vector<DetectionSubDataset<TImg,TObj>> DetectionDataset<TImg,TObj>::splitDataset(std::vector<double> proportions)
{
    assert(proportions.size()>1);
    std::vector<DetectionSubDataset<TImg,TObj>> subsets;
    const std::vector<std::string> sampID = getSamplesID();
    const unsigned int numSamp=sampID.size();
    std::vector<unsigned int> sn; //sub-set sample numbers
    unsigned int sample=0; //sample number
    double sum=0.0;  //cummulative proportion


    // Distribute samples along subsets according to proportions
    for(int i=0;i<proportions.size();i++) {
        sum+=proportions.at(i);
        assert(sum<=1.0);

        //keep adding samples till reach proportion
        while(((double)sample)/numSamp <= sum){
            sn.push_back(sample);
            sample++;
        }

        // reached the proportion. Create SubDataset.
        DetectionSubDataset<TImg,TObj> sd(*this,sn);
        subsets.push_back(sd);

        //prepare next sub-dataset
        sn.clear();
    }
    //done
    return subsets;
}


template<class TImg,typename TObj>
DetectionDataset<TImg,TObj>::~DetectionDataset()
{
}



/****************************************************************************************************/
/****************************************************************************************************/


/// Class that provides access to a subset of a detection dataset.
/** This class provides access to only a subset of samples from a original dataset.
 * The original dataset is stored as a reference, so the subset is only accessible on the scope of the original dataset.
 * The identification of which samples compose the subset is passed on constructor as a vector of samples number or samples ID.
     \param TImg Image type.
    \param TObj Detected object type.

    \ingroup Evaluation
*/
template<class TImg,typename TObj>
class DetectionSubDataset :
        public DetectionDataset<TImg,TObj>
{
public:
    /// Constructructor
    DetectionSubDataset(DetectionDataset<TImg,TObj>& dataset_,const std::vector<unsigned int>& samplesNumber_);
    /// Destructor method.
    virtual ~DetectionSubDataset();

    virtual std::vector<std::string> getSamplesID() const;

    virtual std::shared_ptr<TImg> getImage(const std::string& sampleID) const;

    virtual std::vector<TObj> getObjects(const std::string& sampleID) const ;

private:
    std::vector<unsigned int> samplesNumber;
    DetectionDataset<TImg,TObj>& dataset;

};

template<class TImg,typename TObj>
DetectionSubDataset<TImg,TObj>::DetectionSubDataset(DetectionDataset<TImg,TObj>& dataset,const std::vector<unsigned int>& samplesNumber)
    : dataset(dataset)
    , samplesNumber(samplesNumber)
{

}

template<class TImg,typename TObj>
std::vector<std::string> DetectionSubDataset<TImg,TObj>::getSamplesID() const
{
    /// Gets only a subset from the original dataset
    std::vector<std::string> s;
    std::vector<std::string> orig = dataset.getSamplesID();
    for(const unsigned int i : samplesNumber) {
        s.push_back(orig.at(i));
    }
    return s;
}

template<class TImg,typename TObj>
std::shared_ptr<TImg> DetectionSubDataset<TImg,TObj>::getImage(const std::string& sampleID) const
{
    return dataset.getImage(sampleID);
}

template<class TImg,typename TObj>
std::vector<TObj> DetectionSubDataset<TImg,TObj>::getObjects(const std::string& sampleID) const
{
    return dataset.getObjects(sampleID);
}


/*
Destructor method.
*/
template<class TImg,typename TObj>
DetectionSubDataset<TImg,TObj>::~DetectionSubDataset()
{
}


/****************************************************************************************************/
/****************************************************************************************************/


/// Abstract class for a generic tracking dataset, containing videos and its ground-truth data.
/** Each sample in the dataset has a video and a vector objects for each frame considered as ground-truth.
 * Each sample has an unique ID. A vector with all samples IDs can be retrieved by method 'getSamplesID()'.
 *
    The 'getFrameServer(sampleID)' method returns a frame server related to a video from the dataset
    and the 'getObjects(sampleID)' method returns the respective ground-truth objects.

    Derived classes implementing this interface must implement the three methods mentioned above.

     \param TImg Tipo da imagem.
    \param TObj Tipo do objeto rastreado.

    \ingroup Evaluation
*/
template<class TImg,typename TObj>
class TrackingDataset
{
public:
    /// Destructor method.
    virtual ~TrackingDataset();

    /// Returns a vector of IDs for all samples (videos) in the dataset.
    /// Each sample should be identified by a unique string ID.
    /// This ID can be anything (a number, the name of file, etc). It is used mainly to retrieve a sample
    /// through function getFrameServer and getObjects.
    virtual std::vector<std::string> getSamplesID() const = 0;

    /// Returns a frame server for a sample (video) in the dataset.
    virtual FrameServer<TImg> *getFrameServer(const std::string& sampleID) const = 0;

    /// Returns a vector with the objects in a given sample (video).
    /** The index of the vector is related with the frame number. So position x in this vector
        contains the object of frame x.
    */
    virtual std::vector<TObj> getObjects(const std::string& sampleID) const = 0;
    ///
    //virtual void forEachFrame(const std::string& sampleID, int funct);

    typedef TImg ImgType;
    typedef TObj ObjType;

};

template<class TImg,typename TObj>
TrackingDataset<TImg,TObj>::~TrackingDataset()
{
}



/// Data structure that stores the statistics and evaluation results for a detector.
/** \ingroup Evaluation
    \see DetectorEvaluator
*/
struct DetectorEvalResult
{
    /// Number of detected objects
    unsigned int detectionCount;

    /// Number of real objects (ground truth)
    unsigned int groundTruthCount;

    /// Number of false positives (detections that do not match any ground-truth annotation)
    unsigned int falsePosCount;

    /// Number of false negatives (objects that exist in the ground truth but have not been detected)
    unsigned int falseNegCount;

    /// Number of absolute false positives (detected objects that do not have any similarity with the ground-truth)
    unsigned int absFnCount;

    /// Number of absolute false negatives (ground-truth annotations that are not associated to any detection)
    unsigned int absFpCount;

    /// Number of true positives
    unsigned int truePosCount;

    /// Vector containing the similarity scores for the pair detection-ground-truth
    std::vector<double> similarityScores;

    /// Recall (Sensitivity/ Hit Rate) \n
    /// \f$ TPR = \frac{TP}{P} \f$
    double recall;

    /// Precision or positive predictive value (correct detections) \n
    /// \f$ PPV = \frac{TP}{TP+FP} \f$
    double precision;

    /// F1 score (is the harmonic mean of precision and recall) \n
    /// \f$ F_{1} = \frac{2 \times PPV \times TPR}{PPV + TPR} \f$
    double f1score;

    /// False discovery rate (FDR) \n
    /// \f$ FDR = \frac{FP}{FP+TP}\f$
    double fdr;

    /// False negative rate (FNR) \n
    /// \f$ FRN = \frac{FN}{FN+TP} \f$
    double fnr;

    /// Mean similarity
    double meanSimilarity;

    /// Default constructor
    DetectorEvalResult()
        : detectionCount(0)
        , groundTruthCount(0)
        , falsePosCount(0)
        , falseNegCount(0)
        , absFpCount(0)
        , absFnCount(0)
        , truePosCount(0)
        , similarityScores()
        , recall(-1)
        , precision(-1)
        , fdr(-1)
        , fnr(-1)
        , meanSimilarity(0.0)
    {    };

};


/// Provides algorithms for performance evaluation of detectors.
/** There are two forms of running the evaluation of a detector:
 *
 * 1) Manual: For each image in a test set, call a detector to get a vector of detected objects. Next, you should get the respective ground-truth
 * for the same image. Add the pair of detected objects and ground-truth to the evaluator calling the function 'addTestPoint'.
 * After repeating this procedure for all images in the test set, call function 'computeResults' to get the evaluation statistics.
 * A report can be printed calling 'printReport'.
 *
 * 2) Automatic: Use function 'evaluateDetector' to evaluate a detector (object of class Detector) over a dataset (object of class DetectionDataset).
 * This method will execute the steps in the manual mode for all images in the dataset.
 *
 * Before executing the evaluation, it is necessary to define a similarity function by calling 'setSimilarityFunction'.
 * This function computes a similarity score between a detected object and a ground-truth object.
 *
 * TODO: add examples
 *
 *\param TImg Image type.
 *\param TObj1 Type returned by the detector.
 *\param TObj2 Type for the ground-truth. Usually - and by default - it is the same of TObj1, but could be different.
 *\ingroup Evaluation
*/
template<class TImg,class TObj1,class TObj2 = TObj1>
class DetectorEvaluator
{
    /// A testPoint is defined as a pair containing the detections and the associated ground truth.
    typedef  std::pair<std::vector<TObj1>,std::vector<TObj2>> TestPoint;

private:
    /// Vector containing all the ground-truth and detections.
    std::vector<TestPoint> data;

    /// Function that gives the similarity between a detection and a ground-truth annotation
    std::function<double(TObj1&,TObj2&)> simFcn;

    /// Decision Threshold
    /// If the similarity function between a detected region and a ground truth annotation is greater than the threshold, it will b considered a true positive \n
    // \f$ D_{i} == GT_{i} \text{ if } similarity \geq threshold \f$
    double threshold;

public:
    /// Default Constructor
    DetectorEvaluator()
        :threshold(0.5)
    {

    }

    /// Add a detection-groundtruth pair.
    void addTestPoint(std::vector<TObj1>& detResult, std::vector<TObj2>& groundTruth) {
        TestPoint d(detResult,groundTruth);
        data.push_back(d);
    }

    /// Set a similarity function between a detection and ground truth annotation.
    /// If the similarity function between a detected region and a ground truth annotation
    /// is greater than the threshold, it will be considered a true positive.
    void setSimilarityFunction(std::function<double(TObj1&,TObj2&)> similarityFcn) {
        simFcn=similarityFcn;
    }

    /// Set the similarity threshold to decide if two objects are the same.
    /// If the similarity function between a detected region and a ground truth annotation
    /// is greater than the threshold, it will be considered a true positive.
    void setThreshold(const double t)
    {
        threshold = t;
    }

    /// Compute the evaluation metrics using the entries provided via 'addTestPoint' method.
    /// Using this 'computeResult' the evaluator considers a one to one relation, using the Hungarian Algorithm to find the optimal associations.
    /// For instance, that is the same approach which has been used in "FDDB - A Benchmark for Face Detection in Unconstrained Settings" - Jain, 2010
    DetectorEvalResult computeResult() {

        DetectorEvalResult result;

        for(TestPoint& p : data){ //para cada testPoint

            // CALCULA A SIMILARIDADE DE CADA DETECÇÃO PARA CADA GROUNDTRUTH
            int countD=0;
            int countGT=0;
            const unsigned sizeD = p.first.size();
            const unsigned sizeGT = p.second.size();
            result.detectionCount+=sizeD;
            result.groundTruthCount+=sizeGT;
            //W é uma matriz de similaridade onde as linhas são relacionadas as detecções e as colunas aos groud truths
            double* W = new double[sizeD*sizeGT];
            for(TObj1& d : p.first){  //para cada detecção
                countGT=0;
                for(TObj2& gt : p.second){ //para cada groundTruth
                    double similarity = simFcn(d,gt);
                    W[countD*sizeGT+countGT]=similarity;
                    countGT++;
                }
                countD++;
            }

            // ATRELA UMA DETECÇÃO A UM GROUNDTRUTH MAXIMIZANDO A SOMA DE SIMILARIDADES (CASAMENTOS ÓTIMOS)
            /* Por exemplo, A=[2 -1 0] significa que a detecção 0 foi casada ao groundTruth 2, a detecção 1 não teve casamento
            e a detecção 3 foi casada ao groundTruth 0*/
            std::vector<int> A = computeAssignmentOptimal(W,sizeD,sizeGT);


            // CALCULA A QUANTIDADE DE FALSOS POSITIVOS ABSOLUTOS E FALSOS NEGATIVOS ABSOLUTOS
            unsigned int afp=std::count(A.begin(),A.end(),-1);
            result.absFpCount+=afp;

            unsigned int afn=0;
            for(unsigned int i=0;i<sizeGT;i++){
                if(std::find(A.begin(),A.end(),i) == A.end())
                    afn++;
            }
            result.absFnCount+=afn;

            // ADICIONA SIMILARIDADES AO VETOR similarityScores
            for(unsigned int detIndex=0;detIndex<sizeD;detIndex++){
                int gtIndex=A.at(detIndex);
                if(gtIndex>=0) {//se detecção foi atrelada
                    result.similarityScores.push_back(W[detIndex*sizeGT+gtIndex]);
                    result.meanSimilarity += W[detIndex*sizeGT+gtIndex];
                }
            }

            delete[] W;
        }

        // CALCULA ESTATÍSTICAS
        result.falseNegCount = result.absFnCount;
        result.falsePosCount = result.absFpCount;
        for(const double& s : result.similarityScores){
            if(s<threshold){
                result.falseNegCount++;
                result.falsePosCount++;
            }
            else{
                result.truePosCount++;
            }
        }


        /// Computed statistics:
        /// Precision / PPV = relevant intersection  retrieved / retrieved
        /// (precision is the fraction of retrieved documents that are relevant to the find)
        result.precision = (double) result.truePosCount / (double)result.detectionCount;
        /// Recall/ sensitivity/ Hit rate/ TPR = relevant intersection  retrieved  / relevant (percent of all relevant)
        /// Due the fact that we work with a 1-to-N cardinality we had to adapt the equation from
        /// TPR = TP / (TP + FN) to TPR = (GT - FN)/GT that is quite the same but written in another way
        result.recall = ((double) result.groundTruthCount - (double)result.falseNegCount) / (double)result.groundTruthCount;
        /// F1 score is the harmonic mean of precision and sensitivity
        result.f1score = (double) 2*(double) result.truePosCount/ ((2*(double) result.truePosCount) + (double) result.falsePosCount + (double) result.falseNegCount);
        /// False discovery rate (FDR)
        result.fdr = (double) result.falsePosCount / ((double) result.falsePosCount + (double) result.truePosCount);
        /// False negative rate (FNR)
        result.fnr = 1 - result.recall;
        /// Mean similarity
        result.meanSimilarity = result.meanSimilarity/(double)result.detectionCount;
        //return
        return result;

    }

    /// Compute the evaluation metrics using the entries provided via 'addTestPoint' method.
    /// Using this 'computeResult' the evaluator considers a N-N relation where one ground truth annotation can be associated with more than one detection and vice-versa.
    /// Considering a many to many to many relation may be useful when you can precisely define where a object starts or ends.
    /// For instance, that is the same approach as in "Sun database Large-scale scene recognition from abbey to zoo" - Xiao, 2010.
    DetectorEvalResult computeResultManyToMany() {

        DetectorEvalResult result;

        for(TestPoint& p : data){ //For Each testPoint

            int countD=0;
            int countGT=0;
            const int sizeD = p.first.size();
            const int sizeGT = p.second.size();
            result.detectionCount+=sizeD;
            result.groundTruthCount+=sizeGT;

            //W é uma matriz de similaridade onde as linhas são relacionadas as detecções e as colunas aos groud truths
            double* W = new double[sizeD*sizeGT];
            for(TObj1& d : p.first){  //for each detection
                countGT=0;
                for(TObj2& gt : p.second){ //for each item in the ground truth
                    double similarity = simFcn(d,gt);
                    W[countD*sizeGT+countGT]=similarity;
                    countGT++;
                }
                countD++;
            }

            // COMPUTE THE ABSOLUTE FALSE POSITIVE AND ABSOLUTE FALSE NEGATIVE (NO SIMILARITY)
            // Using a 1-to-N approach we check if each detection has an associated ground truth
            for(int i = 0; i< sizeD; i++){
                bool is_fp = true;
                for(int j = 0; j < sizeGT; j++){
                    // has an associated ground truth annotation
                    if(W[i*sizeGT+j] > 0.0f){
                        result.meanSimilarity += W[i*sizeGT+j];
                        is_fp = false;
                        break;
                    }
                }

                if(is_fp) // absolute false positive found
                    result.absFpCount ++;
            }

            // Using a 1-to-N approach we check if each ground truth annotation has at least one associated detection
            for(int j = 0; j < sizeGT; j++){
                bool is_fn = true;
                for(int i = 0; i< sizeD; i++){
                    // ground truth has an associated detection
                    if(W[i*sizeGT+j] > 0.0f){
                        is_fn = false;
                        break;
                    }
                }

                if(is_fn) // absolute false negative found
                    result.absFnCount ++;
            }


            // COMPUTE THE TRUE POSITIVE, FALSE POSITIVE AND FALSE NEGATIVE (similarity < threshold)
            // For each detection
            for(int i = 0; i< sizeD; i++){
                bool is_fp = true;
                for(int j = 0; j < sizeGT; j++){
                    //if the intersection is greater or equal than the defined threshold
                    if(W[i*sizeGT+j] >= threshold){
                        is_fp = false;
                        break;
                    }
                }

                if(is_fp)
                    result.falsePosCount ++;
                else // is detected and has the necessary similarity
                    result.truePosCount ++;
            }

            /// For each ground truth
            for(int j = 0; j < sizeGT; j++){
                bool is_fn = true;
                for(int i = 0; i< sizeD; i++){
                    //if the intersection is greater or equal than the defined threshold
                    if(W[i*sizeGT+j] >= threshold){
                        is_fn = false;
                        break;
                    }
                }

                if(is_fn){
                    result.falseNegCount ++;
                }
            }

        } // end of the test point loop

        /// Computed statistics:
        /// Precision / PPV = relevant intersection  retrieved / retrieved
        /// (precision is the fraction of retrieved documents that are relevant to the find)
        result.precision = (double) result.truePosCount / (double)result.detectionCount;
        /// Recall/ sensitivity/ Hit rate/ TPR = relevant intersection  retrieved  / relevant (percent of all relevant)
        /// Due the fact that we work with a 1-to-N cardinality we had to adapt the equation from
        /// TPR = TP / (TP + FN) to TPR = (GT - FN)/GT that is quite the same but written in another way
        result.recall = ((double) result.groundTruthCount - (double)result.falseNegCount) / (double)result.groundTruthCount;
        /// F1 score is the harmonic mean of precision and sensitivity
        result.f1score = (double) 2*(double) result.truePosCount/ ((2*(double) result.truePosCount) + (double) result.falsePosCount + (double) result.falseNegCount);
        /// False discovery rate (FDR)
        result.fdr = (double) result.falsePosCount / ((double) result.falsePosCount + (double) result.truePosCount);
        /// False negative rate (FNR)
        result.fnr = 1 - result.recall;
        /// Mean similarity
        result.meanSimilarity = result.meanSimilarity/(double)result.detectionCount;
        //return
        return result;
    }

    /// Imprime um relatório básico dos resultados.
    static void printReport(const DetectorEvalResult& result, std::ostream& out = std::cout) {
        out<<"\n\nReport of the detector tests\n\n";
        out<<"Precision (PPV):  "					<< result.precision <<"\n";
        out<<"Recall/Sensitivity (TPR):  "			<< result.recall <<"\n";
        out<<"False Discovery Rate (FDR):  "		<< result.fdr <<"\n";
        out<<"False Negative Rate (FNR):  "			<< result.fnr <<"\n";
        out<<"F1 Score:  "							<< result.f1score <<"\n";
        out<<"Number of objects in the GT:  "       << result.groundTruthCount <<"\n";
        out<<"Number of detected objects:  "		<< result.detectionCount <<"\n";
        out<<"Absolute false positive:  "			<< result.absFpCount <<"\n";
        out<<"False positives: "					<< result.falsePosCount <<"\n";
        out<<"Absolute false negatives:  "			<< result.absFnCount <<"\n";
        out<<"False Negatives:  "					<< result.falseNegCount <<"\n";
        out<<"True Positives:  "					<< result.truePosCount <<"\n";
        out<<"Mean Similarity: "					<< result.meanSimilarity << "\n";
    }

    /// Runs a detector over each dataset image and compute the performance statistics, printing the result in the default output.
    DetectorEvalResult evaluateDetector(const Detector<TImg,TObj1>& det, DetectionDataset<TImg,TObj2>& dataset,std::function<double(TObj1&,TObj2&)> match)  {

        this->setSimilarityFunction(match);

        // For each sample in dataset
        dataset.forEachSample([&](TImg& img,std::vector<TObj2>& objects,std::string& sampleID) {
            // Detect objects in image and add testPoint
            std::vector<TObj1> detected = det.detect(img);
            this->addTestPoint(detected,objects);
        });
        DetectorEvalResult result = this->computeResult();
        this->printReport(result);
        return result;
    }

    /// Runs a detector over each dataset image and compute the performance statistics based on a many-to-many matching criteria.
    /// Prints the result in the default output.
    DetectorEvalResult evaluateDetectorManyToMany(const Detector<TImg,TObj1>& det, DetectionDataset<TImg,TObj2>& dataset,std::function<double(TObj1&,TObj2&)> match)  { //std::function<bool(TObj1&,TObj2&)> isSameObject

        this->setSimilarityFunction(match);

        // For each sample in dataset
        dataset.forEachSample([&](TImg& img,std::vector<TObj2>& objects,std::string& sampleID) {
            // Detect objects in image and add testPoint
            std::vector<TObj1> detected = det.detect(img);
            this->addTestPoint(detected,objects);
        });
        DetectorEvalResult result = this->computeResultManyToMany();
        this->printReport(result);
        return result;
    }

    /// Runs the Hungarian algorithm to solve the optimal assignment problem.
    /** The input matrix W stores the similarities between detections (rows) and ground-truths (collumns).
     *
     * The resulting vector A contains the ground-truth indexes assigned to detections.
     *
     * For example A=[2 0 -1 -1] means that detection 0 (index) has been assigned to ground-truth 2,
     * detection 1 to ground-truth 0 and detection 2 and 3 have not been assigned to any ground-truth (false positive).
     * Note that ground-truth 1 was not assigned to any detection (false negative).
    */
    static std::vector<int> computeAssignmentOptimal(double *W,int sizeD,int sizeGT){
        std::vector<int> A(sizeD,-1); //"assignment" ou atrelação para cada detecção

        // O algoritmo não funciona quando o número de linhas é maior que de colunas, então preenche com zeros (e o formato de matriz)
        double** m = array_to_matrix(W,sizeD,sizeGT);
        int rowsM = sizeD;
        int colsM = sizeGT;
        if(sizeD>sizeGT)
            colsM=rowsM;

        hungarian_t p;

        /* initialize the gungarian_problem using the cost matrix*/
        hungarian_init(&p, m, rowsM,colsM, HUNGARIAN_MAX);

        hungarian_solve(&p);

        //hungarian_print_assignment(&p);

        for(int i=0;i<sizeD;i++){
            int assignment = p.a[i];
            if(assignment<=(sizeGT-1)){ // if a detection was not assigned to an added column
                if(W[i*sizeGT+assignment]>0) // and the weight is not zero
                    A.at(i)=assignment;
            }
        }

        //TODO: delete m ?
        hungarian_fini(&p);
        return A;
    }

private:

    /// Sort a vector and returns its respective indexes.
    std::vector<size_t> sort_indexes(const std::vector<double> &v) {

        // initialize original index locations
        std::vector<size_t> idx(v.size());
        for (size_t i = 0; i != idx.size(); ++i) idx[i] = i;

        // sort indexes based on comparing values in v
        std::sort(idx.begin(), idx.end(),
                  [&v](size_t i1, size_t i2) {return v[i1] < v[i2];});

        return idx;
    }

    /// Solves the assignment problem by a greedy algorithm (non-optimal).
    /** The input matrix W stores the similarities between detections (rows) and ground-truths (collumns).
     *
     * The resulting vector A contains the ground-truth indexes assigned to detections.
     *
     * For example A=[2 0 -1 -1] means that detection 0 (index) has been assigned to ground-truth 2,
     * detection 1 to ground-truth 0 and detection 2 and 3 have not been assigned to any ground-truth (false positive).
     * Note that ground-truth 1 was not assigned to any detection (false negative).
    */
    std::vector<int> computeAssignmentNonOptimal(double *W,int sizeD,int sizeGT){

        std::vector<int> A(sizeD,-1); //"assignment" ou atrelação para cada detecção

        // Para cada detecção, faz um sort das similaridades e acha o indice do groundTruth com maior valor que ainda não foi atrelado a outra detecção.
        for(int i=0;i<sizeD;i++){
            std::vector<double> similarities(&W[i*sizeGT],&W[i*sizeGT+sizeGT]);
            std::vector<size_t> sortIndex = sort_indexes(similarities);

            for (std::vector<size_t>::reverse_iterator indexIt = sortIndex.rbegin(); indexIt!= sortIndex.rend(); ++indexIt){
                //verifica se o groundTruth com indice *indexIt ainda não foi atrelado a uma detecção
                if(std::find(A.begin(),A.end(),*indexIt) == A.end()){
                    //se a similaridade for zero, atrela a -1 significando que não houve matching com nenhum groundTruth
                    int index = int(*indexIt);
                    if(similarities.at(index) != 0.0)
                        A.at(i)=index;
                    break;
                }
            }
        }
        return A;
    }
};  // class DetectorEvaluator


////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////
///
/// Data structure that stores the statistics and evaluation data for a binary classifier.
/**
\ingroup Evaluation
\see BinaryClassifierEvaluator
*/
struct BinaryClassifierEvalResult
{
    /// Total number of tested instances
    unsigned int detectionCount;

    /// Number of negative instances
    unsigned int negCount;

    /// Number of positive instances
    unsigned int posCount;

    /// Number of false positives
    unsigned int falsePosCount;

    /// Number of false negatives
    unsigned int falseNegCount;

    /// Number of true positives
    unsigned int truePosCount;

    /// Number of true negatives
    unsigned int trueNegCount;

    /// Accuracy \n
    /// \f$ ACC = \frac{TP+TN}{P+N} \f$
    double accuracy;

    /// Recall (Sensitivity/ Hit Rate) \n
    /// \f$ TPR = \frac{TP}{P} \f$
    double recall;

    /// Specificity \n
    /// \f$ SPC = \frac{TN}{N} \f$
    double specificity;

    /// Precision or positive predictive value (correct detections) \n
    /// \f$ PPV = \frac{TP}{TP+FP} \f$
    double precision;

    /// Negative predictive value (the percent of negatives that are correctly classified) \n
    /// \f$ NPV = \frac{TN}{TN+FN} \f$
    double npv;

    /// Fall-out or false positive rate \n
    /// \f$ FPR = \frac{FP}{N} \f$
    double fallout;

    /// False discovery rate \n
    /// \f$ FDR = \frac{FP}{FP+TP} \f$
    double fdr;

    /// False negative rate  \n
    /// \f$ FNR = \frac{FN}{FN + TP} \f$
    double fnr;

    /// Matthews correlation coefficient is used in machine learning as a measure of the quality of binary (two-class) classifications. \n
    /// \f$ MCC = \frac{TP \times TN - FP \times FN}{\sqrt{(TP+FP)(TP+FN)(TN+FP)(TN+FN)}} \f$
    double mcc;

    /// F1 score (is the harmonic mean of precision and recall) \n
    /// \f$ F_{1} = \frac{2 \times PPV \times TPR}{PPV + TPR} \f$
    double f1score;

    /// Default contructor.
    BinaryClassifierEvalResult()
        : detectionCount(0)
        , posCount(0)
        , negCount(0)
        , falsePosCount(0)
        , falseNegCount(0)
        , truePosCount(0)
        , trueNegCount(0)
        , accuracy(-1)
        , recall(-1)
        , precision(-1)
        , npv(-1)
        , fallout(-1)
        , fdr(-1)
        , fnr(-1)
        , mcc(-1)
        , f1score(-1)
    {    };

};


/**
 *  Class that performs a binary classifier evaluation.
There are two different ways to perform a evaluation of a binary classifier (or a detector as a classifier):

1) Manual: For each image in the test set, run the detector/classifier and use the detector <stront>output + grountruth</stron> as input for the addTestPoint. A TestPoint is usually one frame. You have to convert the output to boolean.
To compute the evaluation results use the funcion 'computeResults'. To print the output use the 'printReport' function.

2) Auto: Using the 'evaluateDetectorAsClassifier' function to perform a evaluation of the whole dataset (DetectionDataset class).
The method will assume that a empty vector is a negative and any non-empty vector is a positive. It executes all the steps above.

*/
class BinaryClassifierEvaluator
{
    /// A TestPoint is defined as a pair containing the result of a detection and the associated ground-truth.
    typedef  std::pair<bool,bool> TestPoint;

private:
    /// List that contains all the detections and groundTruths.
    std::list<TestPoint> data;


public:
    /// Default Constuctor
    BinaryClassifierEvaluator()
    {	};

    /// Add a detection - ground truth pair.
    void addTestPoint(bool detResult, bool groundTruth) {
        TestPoint d(detResult,groundTruth);
        data.push_back(d);
    };

    /// Compute the results using the data provided via 'addTestPoint'.
    BinaryClassifierEvalResult computeResult() {
        // Implement the binary classifier evaluation
        BinaryClassifierEvalResult result;

        for(TestPoint& p : data){ //For Each testPoint
            bool p_gt = p.second;
            bool p_co = p.first;

            // Groud-truth statistics
            result.detectionCount ++;
            if(p_gt)
                result.posCount ++;
            else
                result.negCount ++;

            // BinaryClassifier statistics
            if(p_gt && p_co)
                result.truePosCount ++;
            else if (!p_gt && p_co)
                result.falsePosCount ++;
            else if (p_gt && !p_co)
                result.falseNegCount ++;
            else if (!p_gt && !p_co)
                result.trueNegCount ++;

        } // end of the test point loop

        /// Computed statistics:
        /// Accuracy \n
        /// \f$ ACC = \frac{TP+TN}{P+N} \f$
        result.accuracy = (double)(result.truePosCount+result.trueNegCount)/(double)result.detectionCount;

        /// Recall (Sensitivity/ Hit Rate) \n
        /// \f$ TPR = \frac{TP}{P} \f$
        result.recall = (double)result.truePosCount/(double)result.posCount;

        /// Specificity \n
        /// \f$ SPC = \frac{TN}{N} \f$
        result.specificity = (double)result.trueNegCount/(double)result.negCount;

        /// Precision or positive predictive value (correct detections) \n
        /// \f$ PPV = \frac{TP}{TP+FP} \f$
        result.precision = (double)result.truePosCount/(double)(result.truePosCount + result.falsePosCount);

        /// Negative predictive value (the percent of negatives that are correctly classified) \n
        /// \f$ NPV = \frac{TN}{TN+FN} \f$
        result.npv = (double)result.trueNegCount/(double)(result.trueNegCount + result.falseNegCount);

        /// Fall-out or false positive rate \n
        /// \f$ FPR = \frac{FP}{N} \f$
        result.fallout = (double)result.falsePosCount/(double)result.negCount;

        /// False discovery rate \n
        /// \f$ FDR = \frac{FP}{FP+TP} \f$
        result.fdr = (double)result.falsePosCount/(double)(result.falsePosCount+result.truePosCount);

        /// False negative rate  \n
        /// \f$ FNR = \frac{FN}{FN + TP} \f$
        result.fnr = (double)result.falseNegCount/(double)(result.falseNegCount+result.truePosCount);

        /// Matthews correlation coefficient is used in machine learning as a measure of the quality of binary (two-class) classifications. \n
        /// \f$ MCC = \frac{TP \times TN - FP \times FN}{\sqrt{(TP+FP)(TP+FN)(TN+FP)(TN+FN)}} \f$
        result.mcc = (double)(result.truePosCount*result.trueNegCount - result.falsePosCount*result.falseNegCount)/
                (double) sqrt(
                    ((double)result.truePosCount+(double)result.falsePosCount)*((double)result.truePosCount+(double)result.falseNegCount)*((double)result.trueNegCount+(double)result.falsePosCount)*((double)result.trueNegCount+(double)result.falseNegCount)
                    );

        /// F1 score (is the harmonic mean of precision and recall) \n
        /// \f$ F_{1} = \frac{2 \times PPV \times TPR}{PPV + TPR} \f$
        result.f1score = (2*result.precision*result.recall)/(result.precision+result.recall);

        //return
        return result;
    }

    /// Print the report for a binary classifier evaluation. By default recall and precision metrics are given considering the 'true' samples.
    static void printReport(const BinaryClassifierEvalResult& result, std::ostream& out = std::cout) {
        out<<"\n\nReport of the detector tests\n\n";
        out<<"Accuracy:  "							<< result.accuracy <<"\n";
        out<<"Precision (PPV):  "					<< result.precision <<"\n";
        out<<"Recall/Sensitivity (TPR):  "			<< result.recall <<"\n";
        out<<"Specificity (SPC):  "					<< result.specificity <<"\n";
        out<<"Negative Predictive Value (NPV):  "	<< result.npv <<"\n";
        out<<"Fall Out (FPR):  "					<< result.fallout <<"\n";
        out<<"False Discovery Rate (FDR):  "		<< result.fdr <<"\n";
        out<<"Miss Rate (FNR):  "					<< result.fnr <<"\n";
        out<<"F1 Score:  "							<< result.f1score <<"\n";
        out<<"Matthews correlation coefficient (MCC):  " << result.mcc <<"\n";
        out<<"Number of instances:  "				<< result.detectionCount <<"\n";
        out<<"\tPositive instances:  "				<< result.posCount <<"\n";
        out<<"\tNegative instances:  "				<< result.negCount <<"\n";
        out<<"False positives: "					<< result.falsePosCount <<"\n";
        out<<"False Negatives:  "					<< result.falseNegCount <<"\n";
        out<<"True Positives:  "					<< result.truePosCount <<"\n";
        out<<"True Negatives:  "					<< result.trueNegCount <<"\n";
    };


    /// Helper function that iterates over a dataset by calling the detector for each image and comparing the result with the ground truth.
    /// By default, if a image has groundtruth annotations it is considered as a positive instance. Elsewhere, it is considered as a negative instance.
    /// \param TImg Tipo de imagem.
    /// \param TObj1 Detector output object type.
    /// \param TObj2 Ground truth annotation object type

    template<class TImg,class TObj1,class TObj2 >
    BinaryClassifierEvalResult evaluateDetectorAsClassifier(const Detector<TImg,TObj1>& det, DetectionDataset<TImg,TObj2>& dataset)  {


        // For each sample in dataset
        dataset.forEachSample([&](TImg& img,std::vector<TObj2>& objects,std::string& sampleID) {
            // Detect objects in image and add testPoint
            std::vector<TObj1> detected = det.detect(img);
            bool predicted = detected.size() != 0;
            bool ground = objects.size() != 0;
            this->addTestPoint(predicted,ground);
        });

        BinaryClassifierEvalResult result = this->computeResult();
        this->printReport(result);
        return result;
    };

private:
    ;

};

/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////

/// Data structure that stores the statistics and evaluation results for a tracker.
/** \ingroup Evaluation
    \see TrackerEvaluator
*/
struct TrackingEvalResult
        : public DetectorEvalResult
{

};



/// Provides algorithms for performance evaluation of trackers.
/** There are two forms of running the evaluation of a tracker:
 *
 * 1) Manual: For each video in a test set, execute the tracker and store the results in a vector where
 * each position stores the result of a frame. Extract the ground-truth for this video as a vector as well.
 * Use function 'addTestPoint' to add these results into the evaluator.
 * After doing this for all videos in the test set, call function 'computeResults' which will compute several
 * performance statistics.
 *
 * 2) Automatic: Use function 'evaluateTracker' to evaluate a tracker (object of class Tracker) over a dataset (object of class TrackingDataset).
 * This method will execute the steps in the manual mode for all videos in the dataset.
 *
 * Before executing the evaluation, it is necessary to define a similarity function by calling 'setSimilarityFunction'.
 * This function computes a similarity score between a tracked object and a ground-truth object.
 *
 * TODO: add examples
 *\param TImg Image type.
 *\param TObj1 Type returned by the detector.
 *\param TObj2 Type for the ground-truth. Usually - and by default - it is the same of TObj1, but could be different.
 *\ingroup Evaluation
*/
template<class TImg,class TObj1,class TObj2 = TObj1>
class TrackerEvaluator
{
    /// A testPoint is defined as containing the tracking result and its respective ground-truth for a video
    /**  O resultado do rastreamento é representado por um vetor contendo os resultados para cada frame.
    Em cada frame, o rastreador pode estar rastreado vários objetos, portanto, o resultado de cada frame é
    um vetor de objetos do tipo TObj1. O ground-truth também é armazenado em um vetor de forma similar.
    Como o índice do vetor está associado ao número do frame no video, estes dois vetores devem ter o mesmo tamanho.
    */
    struct TestPoint{
        std::vector<std::vector<TObj1>> trackResult;
        std::vector<std::vector<TObj2>> groundTruth;
    };

private:
    /// Each item in this vector contains the tracking result and ground-truth for a video.
    std::vector<TestPoint> data;

    /// Function that computes a similarity between a tracked object and a ground-truth.
    std::function<double(TObj1&,TObj2&)> simFcn;

    /// Limiar de decisão (se a similariedade entre dois objetos for maior que este limiar eles são considerados iguais)
    double threshold;

public:
    /// Default construtor.
    TrackerEvaluator()
        : threshold(0.5)
    {

    }

    /// Add a tracking result and its respective ground-truth.
    void addTestPoint(std::vector<std::vector<TObj1>>& trackResult, std::vector<std::vector<TObj2>>& groundTruth) {
        assert(trackResult.size()==groundTruth.size());

        TrackerEvaluator::TestPoint d;
        d.trackResult = trackResult;
        d.groundTruth = groundTruth;
        data.push_back(d);
    }

    /// Sets the similarity function between tracked object and ground-truth.
    void setSimilarityFunction(std::function<double(TObj1&,TObj2&)> similarityFcn) {
        simFcn=similarityFcn;
    }

    /// Two objects are considered the same if the similarity between them is bigger than this threshold.
    void setThreshold(const double t)
    {
        threshold = t;
    }

    /// Compute performance statistics.
    TrackingEvalResult computeResult() {
        TrackingEvalResult result;

        // Para cada frame de todos os videos, faz a comparação entre os resultados do rastreamento e o ground-truth
        // Cada frame é tratado idependentemente, portanto, usamos o DetectorEvaluator
        DetectorEvaluator<TImg,TObj1,TObj2> detEval;
        detEval.setSimilarityFunction(simFcn);
        detEval.setThreshold(threshold);

        // Para cada video
        for(TestPoint& d : data){
            // Adiciona cada frame como se fosse uma imagem idependente
            for(unsigned long fn=0;fn<d.trackResult.size();fn++)
                detEval.addTestPoint(d.trackResult[fn],d.groundTruth[fn]);
        }
        // Calcula resultados
        DetectorEvalResult detResult = detEval.computeResult();

        result.absFnCount=detResult.absFnCount;
        result.absFpCount=detResult.absFpCount;
        result.detectionCount=detResult.detectionCount;
        result.falseNegCount=detResult.falseNegCount;
        result.falsePosCount=detResult.falsePosCount;
        result.truePosCount=detResult.truePosCount;
        result.groundTruthCount=detResult.groundTruthCount;
        result.similarityScores=detResult.similarityScores;
        return result;
    }

    /// Print an evaluation report.
    static void printReport(const TrackingEvalResult& result, std::ostream& out = std::cout)
    {
        out<<"\n\nRelatório de testes de rastreador\n\n";

        out<<"TODO...";

        DetectorEvaluator<TImg,TObj1,TObj2>::printReport(result,out);

    };
};


/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////


/// FrameServer from a detection dataset.
/**
 *
 * \ingroup Evaluation
 */
template<class TImg,class TObj>
class FrameServerDetData : public FrameServer<TImg>
{
public:
    /// Constructor. A DetectionDataset must be provided.
    FrameServerDetData(DetectionDataset<TImg,TObj>* detData)
        : m_detData(detData)
        , m_forward(true)
    {
        m_samplesIDs=detData->getSamplesID();
        m_sampCount=1;

    }

    /// Destructor method.
    virtual ~FrameServerDetData() { }

    virtual const Frame<TImg> captureFrame(){
        std::string sampID = m_samplesIDs.at(m_sampCount);
        Frame<TImg> frame(m_detData->getImage(sampID),0,m_sampCount);
        m_currentObjects = m_detData->getObjects(sampID);
        if(m_forward)
            m_sampCount++;
        else
            m_sampCount--;
        return frame;

    }

    virtual bool hasNext(){
        if(m_forward)
            return m_sampCount < m_samplesIDs.size();
        else
            return m_sampCount > 0;
    }

    virtual void releaseServer() { }

    /// Returns the objects from current captured object.
    std::vector<TObj> getObjects(){
        return m_currentObjects;
    }

    bool forward() const {return forward;}

    void setForward(bool forward) {m_forward=forward;}

private:
    DetectionDataset<TImg,TObj>* m_detData;
    std::vector<std::string> m_samplesIDs;
    std::vector<TObj> m_currentObjects;
    unsigned int m_sampCount;
    bool m_forward;

};



/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////

/*
struct MultiTrackingEvalResult
{
    //TODO

};

template<class TImg,class TObj1,class TObj2 = TObj1>
class MultiTrackerEvaluator
{
    /// Um testPoint é definido como um par de resultados de uma detecção e seus respectivos ground-truth.
    typedef  std::pair<std::list<TObj1>,std::list<TObj2>> TestPoint;

private:
    /// Lista contendo os resultados e groundTruths.
    std::list<TestPoint> data;

    /// Função que calcula a similariedade entre um objeto detectado e um groundTruth
    std::function<double(TObj1&,TObj2&)> simFcn;
};
*/

} //namespace Evaluation
using namespace VisionCore::Evaluation;
} //namespace VisionCore
#endif // ndef _VISIONEVALUATION_H

