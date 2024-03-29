/*
Copyright 2023 Georgios Titas
Copyright 2023 Alexander Douglas
Copyright 2023 Jijo Varghese
Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the “Software”), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#include "Camera.h"


/**
 * @brief Construct a new Camera:: Camera object
 * 
 * @param camIdx Defaults to 0. If multiple cameras present can be used to define
 * which one to be used with the old stack.
 * @param res_w Defaults to 640. Camera resolution (width)
 * @param res_h Defaults to 480. Camera resolution (width)
 */
Camera::Camera(int camIdx, double res_w, double res_h)
{

#if NEW_CAM_STACK
    std::cout << " NEW camera stack selected " << std::endl;

    activeCapture.options->video_width = res_w;
    activeCapture.options->video_height = res_h;
    activeCapture.options->framerate = 30;
    activeCapture.options->verbose=true;
    std::cout << "Frame width: " << activeCapture.options->video_width << std::endl;
    std::cout << "     height: " << activeCapture.options->video_height << std::endl;
    std::cout << "Capturing FPS: " << activeCapture.options->framerate << std::endl;
#else
    std::cout << " OLD camera stack selected " << std::endl;

    /* Set Camera Settings */
    CamSettings.camIdx = camIdx;
    /* cv::CAP_V4L2 required to run with OLD camera stack */
    CamSettings.camApi = cv::CAP_V4L2;

    /* Open Camera */
    cv::VideoCapture capture(CamSettings.camIdx, CamSettings.camApi);
    if (!capture.isOpened())
    {
        errCode = ERR_INIT;
        std::cerr << "ERROR "<< errCode <<": Can't initialize camera capture" << std::endl;
    }
    capture.set(cv::CAP_PROP_FRAME_WIDTH, res_w);
    capture.set(cv::CAP_PROP_FRAME_HEIGHT, res_h);
    activeCapture = capture;

    std::cout << "Frame width: " << capture.get(cv::CAP_PROP_FRAME_WIDTH) << std::endl;
    std::cout << "     height: " << capture.get(cv::CAP_PROP_FRAME_HEIGHT) << std::endl;
    std::cout << "Capturing FPS: " << capture.get(cv::CAP_PROP_FPS) << std::endl;
#endif
    
}

/**
 * @brief Main loop used by Camera Thread.
 * Loop ends by passing on frames to the pipeline via a callback
 * 
 */
void Camera::camThreadLoop(){
    while(CamSettings.isOn){

#if NEW_CAM_STACK
        if(!activeCapture.getVideoFrame(currentFrame,1000)){
            std::cout<<"Timeout error"<<std::endl;
        }
#else
        activeCapture.read(currentFrame);

        if (currentFrame.empty()) {
            errCode = ERR_EMPTY_FRAME;
            std::cerr << "ERROR: "<< ERR_EMPTY_FRAME << " blank frame grabbed\n";
        }
#endif
        else{
            /* Here add the callback */
            if (cameraCallback){
                AcePlaysUtils callbackData;
                callbackData.nextFrame = currentFrame;
                cameraCallback->nextCallback(callbackData);
            }
        }
    }
}

/**
 * @brief Register a Callback for the Camera Class
 * 
 * @param cb is a CallbackLinker class pointer
 */
void Camera::registerCallback(CallbackLinker* cb){
    cameraCallback = cb;
}

/**
 * @brief Unregister existing callback
 * 
 */
void Camera::unregisterCallback(){
    cameraCallback = nullptr;
}

/**
 * @brief Method call to start the camera recording thread
 * 
 */
void Camera::startRecording(){
    CamSettings.isOn = true;

#if NEW_CAM_STACK
    activeCapture.startVideo();
#else
    if(!activeCapture.open(CamSettings.camIdx, CamSettings.camApi)){
        errCode = ERR_INIT;
        std::cerr << "ERROR "<< errCode <<": Can't initialize camera capture" << std::endl;
    }
#endif
    /* Start Thread */
    camThread = std::thread(&Camera::camThreadLoop, this);

}

/**
 * @brief Method call to stop the camera recording thread
 * Call when exiting the programme
 * 
 */
void Camera::stopRecording(){
    // CamSettings.isOn = false;
    /* If not using Qt or dont have anything running in main() 
      use .join() method to keep the CameraThread running */
    camThread.join();
}

/**
 * @brief Get latest Error Code
 * 
 * @return int, return enum Err_type error code
 */
int Camera::getErr(){
    return errCode;
}

/**
 * @brief Destroy the Camera:: Camera object
 * 
 */
Camera::~Camera()
{
    CamSettings.isOn = false;
    
#if NEW_CAM_STACK
    activeCapture.stopVideo();
#else
    activeCapture.release();
#endif
}
