#include "camera.h"

/**
 * @brief Construct a new Camera:: Camera object
 * 
 * @param camIdx Defaults to 0. If multiple cameras present can be used to define
 * which one to be used
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
    // CamSettings.camApi = cv::CAP_V4L2;
    CamSettings.camApi = cv::CAP_ANY;

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
            AcePlaysUtils callbackData;
            callbackData.nextFrame = currentFrame;
            cameraCallback->nextCallback(callbackData);
        }
        // int key = cv::waitKey(1);
        // if (key == 27/*ESC*/){break;}
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
    cv::destroyWindow("Frame");
}