

/**
* \file VisionTests/main.h
* \brief This file contains several simple examples using CvWorks methods.
*
* \ingroup Viscv
*
* \example VisionTests/main.cpp
* Contains several examples for CvWorks methods. Each function is a complete example.
*
*/

#include "VisionImplementationCv.h"
#include "FireDetectorCv.h"

using namespace VisionCore;
using namespace Viscv;

// Example showing the execution of a detector.
/* This example runs a ColorBlobDetector, which segments blobs in the image within a
given color range. The blobs are represented by their contour as a vector of points.
*/
void testColorBlobDetector(){
    // Defines a contour
    typedef std::vector<cv::Point> Contours;

    // Create a frame server. By default it will try to open the first webcam.
    FrameServerCv frameServer;

    // Create a color blob detector.
    ColorBlobDetectorHF detector;

    // Creates a window for simple visualisation of results.
    CvImageGUI gui;

    // Loop over frames
    while(frameServer.hasNext()){
        // Capture frame.
        Frame<cv::Mat> frame = frameServer.captureFrame();

        // Perform detection
        std::vector<Contours> blobs = detector.detect(frame.getImg());

        // Visualization of results.
        gui.setImg(frame.getImg());
        gui.draw(blobs);
        gui.show();
    }
}


// Example showing how to create a multi-tracker based on a detector.
/* This example first creates a face detector based on ObjectDetectorCCCv method (which is a
 * wrap for OpenCv haar cascade detector).
 *
 * Based on this detector, a tracker for multiple faces is created. The DetectorBasedMultiTracker class
 * can keep a track of faces along a video. This class can be used with any generic detector.
 *
 * \see DetectorBasedMultiTracker
*/
void testDetectorBasedTracker(){
    typedef cv::Mat TImg;
    typedef cv::Rect TObj;

    // Create a frame server. By default it will try to open the first webcam.
    FrameServer<TImg> &frameServer = FrameServerCv();

    // Create a face detector
    std::string cascadeFile = "..\\..\\ExternalLibraries\\OpenCV2.4.9\\data\\haarcascades\\haarcascade_frontalface_default.xml";
    Detector<TImg,TObj> &detector = ObjectDetectorCCCv(cascadeFile);

    // Create a multiple face tracker using the generic detector based multi tracker.
    DetectorBasedMultiTracker<TImg,TObj> tracker(&detector);

    // Start processing frames
    CvImageGUI gui;
    while(frameServer.hasNext()){
        // Capture frame
        Frame<TImg> frame = frameServer.captureFrame();

        // Update tracker
        tracker.update(frame);
        auto objects=tracker.getLastTrack();

        // Draw results
        gui.setImg(frame.getImg());
        gui.draw(objects);
        gui.show();
    }
}


// Example showing a multi-tracker based on abstract class AbstractAutoTracker.
/* This example runs a multi-face tracker implemented in class MultiObjectTrackerCCCv.
 *
 * This class is derived from AbstractAutoTracker, which provides a base implementation for automaticly
 * detecting new objects and managing a pool of individual trackers.
 *
 * \see AbstractAutoTracker
 *
*/
void testMultiTracker(){
    typedef cv::Mat TImg;
    typedef cv::Rect TObj;

     // Create a frame server. By default it will try to open the first webcam.
    FrameServer<TImg> &frameServer = FrameServerCv();

    // Create a multiple face tracker.
    std::string cascadeFile = "..\\..\\ExternalLibraries\\OpenCV2.4.9\\data\\haarcascades\\haarcascade_frontalface_default.xml";
    AbstractAutoTracker<TImg,TObj> &track = MultiObjectTrackerCCCv(cascadeFile);

    // Start processing frames
    CvImageGUI gui;
    while(frameServer.hasNext()){
        // Capture frame.
        Frame<TImg> frame = frameServer.captureFrame();

        // Update tracker
        track.update(frame);
        auto objects=track.getLastTrack();

        // Draw results
        gui.setImg(frame.getImg());
        for(const std::pair<long,TObj>& t : objects)
            gui.draw(t.second);
        gui.show();
    }
}

void testCircleDetector(){
    FrameServer<cv::Mat> &frameServer = FrameServerCv();
    CircleDetectorHTCF circleDetector;

    CvImageGUI gui;
    while(frameServer.hasNext()){
        Frame<cv::Mat> frame = frameServer.captureFrame();
        std::vector<Circle<>> circles = circleDetector.detect(frame.getImg());

        gui.setImg(frame.getImg());
        gui.draw(circles);
        gui.show();
    }
}


// Example showing the execution of a frame server and a detector in separate threads.
/* This example shows how to use the asynchronous execution provided by CvWorks.
 *
 * First a frame server is created and wrapped in a AsyncFrameServerWrap object, which runs it in a
 * individual thread.
 *
 * Then a face detector is created and wrapped in a AsyncDetectorWrap.
 *
 * \see AsyncFrameServerWrap AsyncDetectorWrap
*/
void asyncTest(){
    CvImageGUI gui;

    // Create an AsyncFrameServer from webcam
    FrameServer<cv::Mat> &frameServer = FrameServerCv("E:\\FURG\\Software\\CvWorksRelease1\\Files\\FireDataset\\Car2.mp4");
    AsyncFrameServerWrap<cv::Mat> asyncFS(&frameServer);

    // Set callback when frame is capture. Set the frame for visualization.
    asyncFS.setCallback([&](std::shared_ptr<Frame<cv::Mat>> framePtr){
        gui.setImg(framePtr->getImg());
    });

    // Create an AsyncDetector
    ObjectDetectorCCCv detector("..\\..\\ExternalLibraries\\OpenCV2.4.9\\data\\haarcascades\\haarcascade_frontalface_default.xml");
    AsyncDetectorWrap<cv::Mat,cv::Rect> asyncDet(&asyncFS,&detector);
    asyncDet.sync(true);

    // Set the callback function that is called when objects are detected
    asyncDet.setCallback([&](std::vector<cv::Rect> det){
        std::cout<<"Face detected!!!\n\n";
        gui.draw(det);
        //gui.show(); // doesn't work (probably because gui was created on a different thread)
        cv::imshow("Async Detection",gui.getImg());
        cv::waitKey(1);
    });

    // Start frame server and detector
    asyncDet.start();
    asyncFS.start();

    // Wait for the AsyncFrameServer thread to finish
    asyncFS.getThreadPtr()->join();
    std::cout<<"Finished.";
}


/* Callback function to get a rectangle clicked in a image.*/
cv::Point firstPoint;
cv::Point secondPoint;
int clickCount = 0;
bool rectReady=false;

void onMouse( int event, int x, int y, int, void* )
{
    if( event != cv::EVENT_LBUTTONDOWN )
        return;
    clickCount++;
    if(clickCount==1){
        firstPoint = cv::Point(x,y);
        rectReady=false;
    }
    if(clickCount==2){
        secondPoint = cv::Point(x,y);
        clickCount=0;
        rectReady=true;
    }
}


// Example showing the use of feature matching object detector.
/* This example shows how to use the ObjectDetectorFMCv class.
 *
 * The user has to click on the top left corner and botton right corner
 * to define a rectangle containing the object to be detected.
 * This object is passed to the detector by calling setTargetImage().
 *
 * \see ObjectDetectorFMCv
*/
void testObjectDetectorFMCv(){
    FrameServer<cv::Mat> &frameServer = FrameServerCv();
    ObjectDetectorFMCv detector;

    CvImageGUI gui;
    // defines callback for image click
    cv::setMouseCallback(gui.getWindowName(),onMouse);

    while(frameServer.hasNext()){
        Frame<cv::Mat> frame = frameServer.captureFrame();

        // when two clicks, defines object to detect
        if(rectReady){
            cv::Mat img = frame.getImg();
            cv::Rect ROI(firstPoint,secondPoint);
            detector.setTargetImage(img(ROI));
            rectReady=false;
        }

        //detect
        std::vector<cv::Rect> obj = detector.detect(frame.getImg());

        //shows result
        gui.setImg(frame.getImg());
        gui.draw(obj);
        gui.show();
    }
}


// Example showing the evaluation of a detector.
/* This example shows the evaluation of a face detector.
 * The detector is an ObjectDetectorCCCv object (which is a simple wrap for OpenCv
 * haar cascade detector).
 *
 * The class DatasetFDDB implements interface DetectionDataset, providing generic access to the FDDB face
 * dataset.
 *
 * Then the generic DetectorEvaluator class is used to evaluate the detector.
 *
 * \see DetectorEvaluator DetectionDataset
*/
void testFDDBDataset(){
    // Creates dataset
   DatasetFDDB db("C:/Datasets/FDDB/","C:/Datasets/FDDB/FDDB-fold-01-ellipseList.txt");

   // Creates detector
   ObjectDetectorCCCv det("../../ExternalLibraries/OpenCV2.4.9/data/haarcascades/haarcascade_frontalface_default.xml");

    // Defines the similarity between two rectangles as the proportion of intersection between them.
    std::function<double(const cv::Rect &r1,const cv::Rect &r2)> simFc = [](const cv::Rect &r1,const cv::Rect &r2){
        double areaR1=r1.area();
        double areaR2=r2.area();
        double areaIntrsect=(r1 & r2).area(); //intersection area
        double similarity = areaIntrsect / (areaR1+areaR2-areaIntrsect);
        return similarity;
    };


    // Create the evaluator, passing the dataset, detector and similarity function. A report is printed at the end.
    DetectorEvaluator<cv::Mat,cv::Rect> evaluator;
    evaluator.evaluateDetector(det,db,simFc);

    // The evaluation is done.
    // Just as a visualization example, iterates again over the dataset showing the dataset
    // objects and the result of detections.
    CvImageGUI gui;
    db.forEachSample([&](cv::Mat& img,std::vector<cv::Rect>& objects,const std::string& sampleID){
        gui.setImg(img);
        gui.draw(det.detect(img));
        gui.draw(objects);
        gui.show();
    });
}


// Example showing the generic tracker logger usage.
/* This example shows the usage of TrackerLogger class.
 * First a multiple face tracker is created based on haar cascade classifier.
 *
 * Then it is wrapped on a TrackerLogger, which provides transparent logging functionalities.
 * Note that the tracking with the logger object is performed as any other tracker.
 *
 * After 100 frames the result is saved in a CSV file.
 *
 * \see TrackerLogger
*/
void testDetectionLogger()
{
    // Create frame server from webcam
    FrameServerCv frameServer;

    // Create multiple face tracking
    std::string cascadeFile = "..\\..\\ExternalLibraries\\OpenCV2.4.9\\data\\haarcascades\\haarcascade_frontalface_default.xml";
    MultiObjectTrackerCCCv faceTracker(cascadeFile);

    // Wrap tracker in a TrackerLogger.
    TrackerLogger<cv::Mat,MultiObjectTrackerCCCv::ObjType> tracker(&faceTracker);

    // Start capturing frames
    CvImageGUI gui;
    while(frameServer.hasNext()){
        Frame<cv::Mat> frame = frameServer.captureFrame();

        // Update tracker
        tracker.update(frame);

        // Visualization
        gui.setImg(frame.getImg());
        for(const std::pair<long,cv::Rect>& t : tracker.getLastTrack())
            gui.draw(t.second);
        gui.show();

        // Stop after 100 frames
        if(frame.getNumber()>100)
            break;
    }
    // Write log to a CSV file
    tracker.writeToFileAsCsv("test.txt");
}

// Simple tag detector example.
void testTagDetection(){
    FrameServer<cv::Mat> &frameServer = FrameServerCv();
    ARTagDetectorBLP tagDetector;

    CvImageGUI gui;
    while(frameServer.hasNext()){
        Frame<cv::Mat> frame = frameServer.captureFrame();
        std::vector<ARTag> tags = tagDetector.detect(frame.getImg());

        gui.setImg(frame.getImg());
        gui.draw(tags);
        gui.show();
    }
}

void videoChenebertXProposed(){

    std::function<double(std::vector<cv::Point> &dt, cv::Rect &gt)> simFc = [](std::vector<cv::Point> &dt, cv::Rect &gt){
        cv::Rect dtr = cv::boundingRect(dt);
        double area_dtr= dtr.area();
        double areaIntrsect=(dtr & gt).area();
        double similarity = areaIntrsect/area_dtr;
        return similarity;
    };

    DatasetFire db("../../Files/FireDataset/");

    //Cria evaluator
    TrackerEvaluator<cv::Mat, std::vector<cv::Point>, cv::Rect> evaluator;
    evaluator.setSimilarityFunction(simFc);

    FireTrackerSA tracker("../../Files/FireDataset/rf_classifiers/20150218_region_classification_model",
                          "../../Files/FireDataset/rf_classifiers/color_classification_model");

    CvImageGUI gui;
    for (std::string s: db.getSamplesID()){
        std::cout << s<< std::endl;
        FrameServer<cv::Mat> *fs = db.getFrameServer(s);
        std::vector<std::vector<cv::Rect>> objects = db.getObjects(s);
        while(fs->hasNext()){
            Frame<cv::Mat> f = fs->captureFrame();
            gui.setImg(f.getImg());
            tracker.update(f);
            gui.draw(objects.at(f.getNumber()));
            gui.draw(tracker.getLastTrack());
            gui.show();
            cv::waitKey(1);
        }

    }
}

int main(int argc, char* argv[])
{
    testColorBlobDetector();
    //testCircleDetector();
    //testDetectorBasedTracker();
    //testMultiTracker();
    //testDetectionLogger();
    //asyncTest();
    //testFDDBDataset()
    //testTagDetection();
    //testObjectDetectorFMCv();
    //videoChenebertXProposed();
    system ("PAUSE");
}
