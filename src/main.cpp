#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>  // cv::Canny()
#include <iostream>

#include "camera.h"

using namespace cv;
using std::cout; using std::cerr; using std::endl;

#define BKG_ADAPTIVE_THRESH 50


cv::Mat preprocess_image(cv::Mat image){

    // Returns a grayed, blurred, and adaptively thresholded camera image.
    cv::Mat processed_img;
    cv::cvtColor(image, processed_img, cv::COLOR_BGR2GRAY);
    cv::GaussianBlur(processed_img, processed_img,cv::Size(5,5),0);
    /* Canny Edge Detection filter seems to improve detection of cards with Red coloured suits*/
    cv::Canny(processed_img, processed_img, 100, 200);
    // # The best threshold level depends on the ambient lighting conditions.
    // # For bright lighting, a high threshold must be used to isolate the cards
    // # from the background. For dim lighting, a low threshold must be used.
    // # To make the card detector independent of lighting conditions, the
    // # following adaptive threshold method is used.
    // #
    // # A background pixel in the center top of the image is sampled to determine
    // # its intensity. The adaptive threshold is set at 50 (THRESH_ADDER) higher
    // # than that. This allows the threshold to adapt to the lighting conditions.
    cv::Size img_size = image.size();
    int thresh_level = processed_img.at<uchar>(img_size.height/100, img_size.width/2) + BKG_ADAPTIVE_THRESH;

    cv::threshold(processed_img, processed_img, thresh_level, 256, cv::THRESH_BINARY);
    
    // string filename = samples::findFile(names[i]);

    return processed_img;
    
}


void find_cards(cv::Mat image){

    /* Finds all card-sized contours in a thresholded camera image.
    Returns the number of cards, and a list of card contours sorted
    from largest to smallest. */
    std::vector<std::vector<Point>> contours;
    std::vector<Vec4i> hierarchy;
    cv::findContours(image, contours, hierarchy, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE );
    // Find contours and sort their indices by contour size
    std::sort(contours.begin(), contours.end(), [](const std::vector<Point>& c1, const std::vector<Point>& c2)
    {
        return cv::contourArea(c1, false) < cv::contourArea(c2, false);
    });
    // If there are no contours, do nothing
    if (contours.size() == 0){
        return;
    }    
    std::cout << contours.size() << std::endl;
    // std::cout << index_short << std::endl;
    

    // // Otherwise, initialize empty sorted contour and hierarchy lists
    // cnts_sort = [];
    // hier_sort = [];
    // cnt_is_card = np.zeros(len(cnts),dtype=int);

    // // Fill empty lists with sorted contour and sorted hierarchy. Now,
    // // the indices of the contour list still correspond with those of
    // // the hierarchy list. The hierarchy array can be used to check if
    // // the contours have parents or not.
    // for i in index_sort:
    //     cnts_sort.append(cnts[i])
    //     hier_sort.append(hier[0][i])

    // // Determine which of the contours are cards by applying the
    // // following criteria: 1) Smaller area than the maximum card size,
    // // 2), bigger area than the minimum card size, 3) have no parents,
    // // and 4) have four corners

    // for i in range(len(cnts_sort)):
    //     size = cv2.contourArea(cnts_sort[i])
    //     peri = cv2.arcLength(cnts_sort[i],True)
    //     approx = cv2.approxPolyDP(cnts_sort[i],0.01*peri,True)
        
    //     if ((size < CARD_MAX_AREA) and (size > CARD_MIN_AREA)
    //         and (hier_sort[i][3] == -1) and (len(approx) == 4)):
    //         cnt_is_card[i] = 1

    // return cnts_sort, cnt_is_card
}

class CameraCallback : public CallbackLinker{
    virtual void passFrame(Mat nextFrame){
        
        cv::imshow("Frame", preprocess_image(nextFrame));
        find_cards(nextFrame);
    }
};

int main(int, char**)
{
    cout << "Opening camera..." << endl;
    Camera camera_obj(0,0, 1280, 720);

    CameraCallback show_cam_callback;
    camera_obj.registerCallback(&show_cam_callback);
    camera_obj.startRecording();

 

    camera_obj.stopRecording();
    return 0;
}

