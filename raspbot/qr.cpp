#include "qr.h"
#include <opencv2/opencv.hpp>
#include <opencv2/objdetect.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
#include <regex>
#include <string>

using namespace cv;
using namespace std;

int decodeDisplay(Mat &im){
    vector<QRCodeDetector> detectors;
    QRCodeDetector qrDecoder;
    vector<Point> bbox;
    string data = qrDecoder.detectAndDecode(im,bbox);
    
    if(data.length() > 0){
        cout << "[INFO] Found QR code: " << data << endl;
        polylines(im,bbox,true,Scalar(255,255,255),2);
        return stoi(data);
    }
    else{
        return -1;
    }
}

int capture(){
    VideoCapture cap(0);
    cap.set(CAP_PROP_FRAME_WIDTH, 320);
    cap.set(CAP_PROP_FRAME_HEIGHT, 240);
    cap.set(CAP_PROP_FPS, 30);
    cap.set(CAP_PROP_FOURCC, VideoWriter::fourcc('M','J','P','G'));
    cap.set(CAP_PROP_BRIGHTNESS,40);
    cap.set(CAP_PROP_CONTRAST, 50);
    cap.set(CAP_PROP_EXPOSURE, 156);
    
    if (!cap.isOpened()) {
        std::cerr << "Error: Unable to open the camera" << std::endl;
        return -1;
    } 
    
    int xy;
    Mat frame;
    
    while(true) {
        try{
            cap >> frame;
            if(frame.empty()){
                cerr << "Error reading frame" << endl;
                break;
            }
            
            Mat gray;
            cvtColor(frame, gray, COLOR_BGR2GRAY);
            xy = decodeDisplay(gray);
            if(xy != -1){
                break;
            }
        }
        catch(Exception &e){
            std::cerr << e.what() << std::endl;
            break;           
        }
    }
    cap.release();
    return xy;
}




/*
int capture(){
    cv::VideoCapture cap(0);
    cap.set(CAP_PROP_FRAME_WIDTH, 320);
    cap.set(CAP_PROP_FRAME_HEIGHT, 240);
    if (!cap.isOpened()) {
        std::cerr << "Error: Unable to open the camera" << std::endl;
        return -1;
    }

    cv::QRCodeDetector qrDecoder;
    cv::Mat frame, gray, equalized;
    int xy;
    int error_code = -1;
    try{
        cap >> frame;
        if (frame.empty()) {
            std::cerr << "Error: Unable to capture frame" << std::endl;
            cap.release();
            return -1;
        }

        cv::cvtColor(frame, gray, COLOR_BGR2GRAY);
        //cv::equalizeHist(gray, equalized);
        
        vector<Point> points;
        
        std::string data = qrDecoder.detectAndDecode(gray, points);
        if (!data.empty()) {
            std::cout << data << std::endl;
            cv::polylines(frame, points, true, Scalar(0, 255, 255), 2);
            xy = stoi(data);
            frame.release();
            cap.release();
            return xy;
        }
        else{
            frame.release();
            cap.release();
            return error_code;
        }
    }
    catch(cv::Exception &e){
        std::cerr << e.what() << std::endl;
        cap.release();
        return error_code;
    }
    
}
*/
