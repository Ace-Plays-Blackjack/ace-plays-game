#include <opencv2/core.hpp>
#include <iostream>

#include "Leds.h"
#include "Camera.h"
#include "DetectCard.h"
#include "StrategyEngine.h"
#include "GamePlay.h"
#include <unistd.h>

using namespace cv;
using namespace std;
using std::cout; using std::cerr; using std::endl;

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
    double res_w = 1024;
    double res_h = 768;
    Camera camera_obj(0, res_w, res_h); 

    /* PATH depends on where the executable is called from */
    // DetectCard cards_obj("../../Card_Imgs/"); // this path works for Windows
    DetectCard cards_obj("../Card_Imgs/"); // this path works for Pi
    GamePlay gameplay_obj(res_w, res_h);

    camera_obj.registerCallback(&cards_obj);
    cards_obj.registerCallback(&gameplay_obj);

    cards_obj.startProcessing();
    camera_obj.startRecording();
    
    /* Demonstration of LED Toggling*/
    ToggleLED leds;
    leds.flashled(SPLIT);
    usleep(1000000);
    leds.flashled(HIT);
    usleep(1000000);
    leds.flashled(STOP);

    /* Demonstration of Strategy Engine */
    std::vector<int> vect{ 11, 6, 5, 10};
    StrategyEngine firstdecision;
    decisions choice = firstdecision.getchoice(10, vect);
    
    // camera_obj.stopRecording();
    cards_obj.stopProcessing();
    return 0;
}

