#include "camera.h"

/**
 * @brief Construct a new Camera:: Camera object
 * 
 * @param camIdx Defaults to 0. Finds default connected camera
 * @param camApi Defaults to cv::CAP_ANY. Automatically finds a suitable backend API
 */
Camera::Camera(int camIdx, int camApi)
{
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
    activeCapture = capture;

    std::cout << "Frame width: " << capture.get(cv::CAP_PROP_FRAME_WIDTH) << std::endl;
    std::cout << "     height: " << capture.get(cv::CAP_PROP_FRAME_HEIGHT) << std::endl;
    std::cout << "Capturing FPS: " << capture.get(cv::CAP_PROP_FPS) << std::endl;
}

void Camera::display(){
    cv::imshow("Frame", currentFrame);
}

/**
 * @brief Main loop used by Camera Thread.
 * Loop ends by passing on frames to the pipeline via a callback
 * 
 */
void Camera::camThreadLoop(){
    while(CamSettings.isOn){
        activeCapture.read(currentFrame);

        if (currentFrame.empty()) {
            errCode = ERR_EMPTY_FRAME;
            std::cerr << "ERROR: "<< ERR_EMPTY_FRAME << " blank frame grabbed\n";
            return;
        }

        // Here add the callback
        display();
        int key = cv::waitKey(1);
        if (key == 27/*ESC*/){break;}
    }
}

void Camera::startRecording(){
    CamSettings.isOn = true;
    if(!activeCapture.open(CamSettings.camIdx, CamSettings.camApi)){
        errCode = ERR_INIT;
        std::cerr << "ERROR "<< errCode <<": Can't initialize camera capture" << std::endl;
    }
    
    /* Start Thread */
    camThread = std::thread(&Camera::camThreadLoop, this);
    
}

void Camera::stopRecording(){
    // CamSettings.isOn = false;
    camThread.join();
}

int Camera::getErr(){
    return errCode;
}

Camera::~Camera()
{
    CamSettings.isOn = false;
    activeCapture.release();
}