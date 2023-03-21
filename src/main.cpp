#include <opencv2/core.hpp>
#include <iostream>

#include "camera.h"

using namespace cv;
using namespace std;
using std::cout; using std::cerr; using std::endl;

cv::String folder = "../../Card_Imgs/*.jpg"; // again we are using the Opencv's embedded "String" class
CardTemplate cardTemplates(folder);



class CameraCallback : public CallbackLinker{
    void passFrame(Mat nextFrame){
        cv::imshow("Frame", nextFrame);
        // std::cout << "Num of Cards: " << card_params.num_of_cards << std::endl;

    }
};

int main(int, char**)
{
    cout << "Opening camera..." << endl;
    Camera camera_obj(1,0, 1280, 720);

    CameraCallback show_cam_callback;
    camera_obj.registerCallback(&show_cam_callback);

    camera_obj.startRecording();
    camera_obj.stopRecording();
    return 0;
}

