#include "camera.h"

/**
 * @brief Construct a new Camera:: Camera object
 * 
 * @param camIdx Defaults to 0. Finds default connected camera
 * @param camApi Defaults to cv::CAP_ANY. Automatically finds a suitable backend API
 */
Camera::Camera(int camIdx, int camApi, double res_w, double res_h)
{

#if NEW_CAM_STACK
    activeCapture.options->video_width=res_w;
    activeCapture.options->video_height=res_h;
    activeCapture.options->framerate=30;
    activeCapture.options->verbose=true;
#else
    /* Set Camera Settings */
    CamSettings.camIdx = camIdx;
    CamSettings.camApi = camApi;

    /* Open Camera */
    cv::VideoCapture capture(camIdx, camApi);
    if (!capture.isOpened())
    {
        errCode = ERR_INIT;
        std::cerr << "ERROR "<< errCode <<": Can't initialize camera capture" << std::endl;
    }
    capture.set(cv::CAP_PROP_FRAME_WIDTH, res_w);
    capture.set(cv::CAP_PROP_FRAME_HEIGHT, res_h);
    activeCapture = capture;

#endif
    std::cout << "Frame width: " << capture.get(cv::CAP_PROP_FRAME_WIDTH) << std::endl;
    std::cout << "     height: " << capture.get(cv::CAP_PROP_FRAME_HEIGHT) << std::endl;
    std::cout << "Capturing FPS: " << capture.get(cv::CAP_PROP_FPS) << std::endl;
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
            cameraCallback->passFrame(currentFrame);
        }
        int key = cv::waitKey(1);
        if (key == 27/*ESC*/){break;}
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