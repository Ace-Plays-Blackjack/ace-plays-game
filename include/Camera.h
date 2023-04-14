/*
Copyright 2023 Georgios Titas
Copyright 2023 Alexander Douglas
Copyright 2023 Jijo Varghese
Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the “Software”), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

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

/**
 * @brief Class for initialising a Camera object and capturing frames.
 * 
 */
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
    Camera(int camIdx = 0, double res_w = 640, double res_h = 480);
    ~Camera();
    void registerCallback(CallbackLinker* cb);
    void unregisterCallback();
    void startRecording();
    void stopRecording();
    int getErr();
    
};


#endif /* CAMERA_H */
