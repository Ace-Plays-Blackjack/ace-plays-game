#include <opencv2/core.hpp>
#include <iostream>

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
    cout << "Opening camera..." << endl;
    Camera camera_obj(0,0, 1280, 720);
    DetectCard cards_obj("../../Card_Imgs/");

    CameraCallback show_cam_callback;
    camera_obj.registerCallback(&cards_obj);

    cards_obj.startProcessing();
    camera_obj.startRecording();
    camera_obj.stopRecording();
    return 0;
}

