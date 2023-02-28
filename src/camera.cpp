#include <opencv2/videoio.hpp>
#include <iostream>
#include "camera.h"

Camera::Camera(int camIdx, int camApi)
{

    cv::VideoCapture capture(camIdx, camApi); // open the first camera
    if (!capture.isOpened())
    {
        errCode = ERR_INIT;
        std::cerr << "ERROR "<< errCode <<": Can't initialize camera capture" << std::endl;
    }
    active_capture = capture;
}


int Camera::getErr(){
    return errCode;
}

void Camera::startRecording(){

}

Camera::~Camera()
{
}