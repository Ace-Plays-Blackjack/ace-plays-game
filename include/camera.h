#ifndef CAMERA_H
#define CAMERA_H
#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>

#include <iostream>

enum Err_type{
    NO_ERROR,
    ERR_INIT
};

class Camera
{
private:
    struct{
        int camIdx; 
        int camApi;
    } CamSettings;

    int errCode;
    cv::Mat currentFrame;
    cv::VideoCapture activeCapture;
    void camThread();

public:
    Camera(int camIdx = 0, int camApi = cv::CAP_ANY);
    ~Camera();
    void startRecording();
    void stopRecording();
    int getErr();
    
};





#endif CAMERA_H