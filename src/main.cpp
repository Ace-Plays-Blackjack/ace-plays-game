#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>  // cv::Canny()
#include <iostream>

#include "camera.h"

using namespace cv;
using std::cout; using std::cerr; using std::endl;

class CameraCallback : public CallbackLinker{
    virtual void passFrame(Mat nextFrame){
        cv::imshow("Frame", nextFrame);
    }
};

int main(int, char**)
{
    cout << "Opening camera..." << endl;
    Camera camera_obj;
    CameraCallback show_cam_callback;
    camera_obj.registerCallback(&show_cam_callback);
    camera_obj.startRecording();
    camera_obj.stopRecording();

    return 0;
}