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
    int errCode;
    int camIdx, camApi;
    cv::Mat current_frame;
    cv::VideoCapture active_capture;
    void camThread();

public:
    Camera(int camIdx = 0, int camApi = cv::CAP_ANY);
    ~Camera();
    void startRecording();
    void stopRecording();
    void getErr();
    
};





#endif CAMERA_H