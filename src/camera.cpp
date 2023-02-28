#include <opencv2/videoio.hpp>
#include <iostream>
#include "camera.h"

Camera::Camera(int camIdx, int camApi)
{
    CamSettings.camIdx = camIdx;
    CamSettings.camApi = camApi;
    cv::VideoCapture capture(camIdx, camApi); // open the first camera
    if (!capture.isOpened())
    {
        errCode = ERR_INIT;
        std::cerr << "ERROR "<< errCode <<": Can't initialize camera capture" << std::endl;
    }
    activeCapture = capture;
}

void Camera::startRecording(){

}

int Camera::getErr(){
    return errCode;
}

Camera::~Camera()
{
}