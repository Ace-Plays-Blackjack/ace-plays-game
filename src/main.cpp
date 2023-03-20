#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>  // cv::Canny()
#include <iostream>
<<<<<<< Updated upstream

#include "camera.h"
=======
#include "leds.h"
#include "camera.h"
#include <thread>
#include "strategy.h"

#include <vector>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>
>>>>>>> Stashed changes

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
<<<<<<< Updated upstream
=======
    choice = SPLIT;
    std::vector<int> vect{ 11, 6, 5, 10};
    decisionmaker firstdecision;
    choice = firstdecision.getchoice(10, vect);
    usleep(5000000);
>>>>>>> Stashed changes
    camera_obj.stopRecording();

    return 0;
}