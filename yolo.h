#ifndef YOLO_H
#define YOLO_H
#include "Header.h"

#include <fstream>
#include <sstream>
#include <iostream>
#include <opencv2/dnn/dnn.hpp>
#include <opencv2/imgproc.hpp>
//#include <opencv2/highgui.hpp>

using namespace cv;
using namespace dnn;
using namespace std;




struct Net_config
{
    float confThreshold; // class Confidence threshold
    float nmsThreshold;  // Non-maximum suppression threshold
    float objThreshold;  //Object Confidence threshold
    string netname;
};

class YOLO
{
    public:
        YOLO(Net_config config);
        void detect(Mat& frame);
    private:
        const float anchors[3][6] = {{10.0, 13.0, 16.0, 30.0, 33.0, 23.0}, {30.0, 61.0, 62.0, 45.0, 59.0, 119.0},{116.0, 90.0, 156.0, 198.0, 373.0, 326.0}};
        const float stride[3] = { 8.0, 16.0, 32.0 };
        const string classesFile = "/Users/chenziwei/Qt project/ImageProcessing/coco.name";
        const int inpWidth = 640;
        const int inpHeight = 640;
        float confThreshold;
        float nmsThreshold;
        float objThreshold;

        char netname[20];
        vector<string> classes;
        Net net;
        void drawPred(int classId, float conf, int left, int top, int right, int bottom, Mat& frame);
        void sigmoid(Mat* out, int length);
};

#endif // YOLO_H
