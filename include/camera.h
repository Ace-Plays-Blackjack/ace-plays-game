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
    int err_code;
    cv::Mat current_frame;
    
public:
    cv::VideoCapture active_capture;
    Camera(/* args */);
    ~Camera();
    void start_recording();
    void stop();
    
};





#endif CAMERA_H