#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>  // cv::Canny()
#include <iostream>
#include "leds.h"
#include "camera.h"
#include <thread>

#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>

using namespace cv;
using std::cout; using std::cerr; using std::endl;

class CameraCallback : public CallbackLinker{
    virtual void passFrame(Mat nextFrame){
        cv::imshow("Frame", nextFrame);
    }
};

void flashled(decisions (*gd)()) {
    led_flasher flasher;
	flasher.mainthread(gd);
}

static decisions choice;

decisions getdecision() {
    return choice;

}

int main(int, char**)
{
    std::thread flasher (flashled, getdecision);
    cout << "Opening camera..." << endl;
    Camera camera_obj;
    CameraCallback show_cam_callback;
    camera_obj.registerCallback(&show_cam_callback);
    camera_obj.startRecording();
    usleep(5000000);
    choice = HIT;
    usleep(1000000);
    choice = SPLIT;
    camera_obj.stopRecording();
    flasher.join();
    return 0;
}