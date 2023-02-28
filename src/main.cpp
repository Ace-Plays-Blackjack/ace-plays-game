#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>  // cv::Canny()
#include <iostream>

#include "camera.h"

using namespace cv;
using std::cout; using std::cerr; using std::endl;

int main(int, char**)
{
    cout << "Opening camera..." << endl;
    Camera camera_obj;

    camera_obj.startRecording();
    camera_obj.stopRecording();

    return 0;
}