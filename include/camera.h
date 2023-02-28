#ifndef CAMERA_H
#define CAMERA_H
#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>

#include <iostream>
#include <thread>

enum Err_type{
    NO_ERROR,
    ERR_INIT,
    ERR_EMPTY_FRAME
};

class Camera
{
private:
    struct{
        int camIdx; 
        int camApi;
        bool isOn = false;
    } CamSettings;

    int errCode;
    cv::Mat currentFrame;
    cv::VideoCapture activeCapture;
    std::thread camThread;
    void camThreadLoop();

public:
    Camera(int camIdx = 0, int camApi = cv::CAP_ANY);
    ~Camera();
    //virtual void hasFrame(cv::Mat frame) = 0;
    void display(); // Temporary
    void startRecording();
    void stopRecording();
    int getErr();
    
};




#endif /* CAMERA_H */