#include <opencv2/core.hpp>
#include <iostream>
#include "leds.h"
#include "camera.h"
#include "DetectCard.h"

using namespace cv;
using namespace std;
using std::cout; using std::cerr; using std::endl;


/* Not Used */
// class CameraCallback : public CallbackLinker{
//     void passFrame(cv::Mat &nextFrame){
//         cv::imshow("Frame", nextFrame);
//         // std::cout << "Num of Cards: " << card_params.num_of_cards << std::endl;
//     }
// };


int main(int, char**)
{
    std::cout<<cv::getBuildInformation();
    cout << "Opening camera..." << endl;

/*  Choose one of the resolutions bellow when using the 
    new LIBCAMERA stack with LCCV

    | Size        | FPS     | LCCV CPU usage |
    | ----------- | :-----: | :-----: |
    | 640 X 480   | 15      | **10%** |
    | 640 X 480   | 30      | **17%** |
    | 1024 x 768  | 15      | **16%** |
    | 1280 x 960  | 15      | **18%** |
    | 2592 x 1944 | 15      | **48%** |
    | 3280 x 2464 | 15      | **51%** |
*/ 
    // Camera camera_obj(0,0, 1280, 720);
    
    Camera camera_obj(0, cv::CAP_V4L2, 1024, 768); // cv::CAP_V4L2 required to run on Pi

    /* PATH depends on where the executable is called from */
    //DetectCard cards_obj("../../Card_Imgs/"); // this path works for Windows
    DetectCard cards_obj("../Card_Imgs/"); // this path works for Pi

    camera_obj.registerCallback(&cards_obj);
    cards_obj.startProcessing();
    camera_obj.startRecording();
    camera_obj.stopRecording();
    return 0;
}

