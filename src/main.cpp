#include <opencv2/core.hpp>
#include <iostream>

#include "camera.h"
#include "leds.h"
#include "camera.h"
#include "DetectCard.h"

#include <thread>
#include "strategy.h"

#include <vector>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>


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
    
    // Camera camera_obj(0,0, 1280, 720);
    
    Camera camera_obj(0, cv::CAP_V4L2, 1280, 720); // cv::CAP_V4L2 required to run on Pi

    /* PATH depends on where the executable is called from */
    //DetectCard cards_obj("../../Card_Imgs/"); // this path works for Windows
    DetectCard cards_obj("../Card_Imgs/"); // this path works for Pi

    camera_obj.registerCallback(&cards_obj);
    decisions choice;
    ToggleLED leds;
    choice = SPLIT;
    leds.flashled(choice);
    usleep(5000000);
    choice = HIT;
    leds.flashled(choice);
    cards_obj.startProcessing();
    camera_obj.startRecording();
    
    choice = SPLIT;
    std::vector<int> vect{ 11, 6, 5, 10};
    decisionmaker firstdecision;
    choice = firstdecision.getchoice(10, vect);
    usleep(5000000);
    camera_obj.stopRecording();
    return 0;
}

