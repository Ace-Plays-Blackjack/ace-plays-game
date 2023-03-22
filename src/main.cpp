#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>  // cv::Canny()
#include <iostream>
#include "leds.h"
#include "camera.h"
#include <thread>
#include "strategy.h"

#include <vector>
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


int main(int, char**)
{
    cout << "Opening camera..." << endl;
    Camera camera_obj;
    decisions choice;
    CameraCallback show_cam_callback;
    led_flasher leds;
    camera_obj.registerCallback(&show_cam_callback);
    camera_obj.startRecording();
    choice = SPLIT;
    leds.flashled(choice);
    std::vector<int> vect{ 11, 6, 5};
    decisionmaker firstdecision;
    choice = firstdecision.getchoice();
    leds.flashled(choice);
    usleep(5000000);
    camera_obj.stopRecording();
    return 0;
}
