#ifndef CAMERA_H
#define CAMERA_H
#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>

#include <iostream>
#include <thread>
#include "CallbackLinker.h"

/* Choose the old or new Pi camera stack from CMakeLists.txt file*/
#if defined(CMAKE_NEW_CAM_STACK_ON)
#define NEW_CAM_STACK 1
#elif defined(CMAKE_NEW_CAM_STACK_OFF)
#define NEW_CAM_STACK 0
#else
#define NEW_CAM_STACK 0
#endif

#if NEW_CAM_STACK
#include <lccv.hpp>
#endif


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

#if NEW_CAM_STACK
    lccv::PiCamera activeCapture;
#else
    cv::VideoCapture activeCapture;
#endif

    std::thread camThread;
    void camThreadLoop();
    CallbackLinker* cameraCallback = nullptr;

public:
    Camera(int camIdx = 0, int camApi = cv::CAP_ANY, double res_w = 640, double res_h = 480);
    ~Camera();
    void registerCallback(CallbackLinker* cb);
    void unregisterCallback();
    void startRecording();
    void stopRecording();
    int getErr();
    
};




#endif /* CAMERA_H */