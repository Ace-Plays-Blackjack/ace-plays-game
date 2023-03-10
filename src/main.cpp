#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>  // cv::Canny()
#include <iostream>

#include "camera.h"

using namespace cv;
using namespace std;
using std::cout; using std::cerr; using std::endl;

#define BKG_ADAPTIVE_THRESH 50
#define CARD_MAX_AREA 120000
#define CARD_MIN_AREA 25000
RNG rng(12345);

cv::Mat preprocess_image(cv::Mat image){

    // Returns a grayed, blurred, and adaptively thresholded camera image.
    cv::Mat processed_img;
    cv::cvtColor(image, processed_img, cv::COLOR_BGR2GRAY);
    cv::GaussianBlur(processed_img, processed_img,cv::Size(5,5),0);
    /* Canny Edge Detection filter seems to improve detection of cards with Red coloured suits*/
    cv::Canny(processed_img, processed_img, 50, 200);
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

    // cv::threshold(processed_img, processed_img, thresh_level, 255, cv::THRESH_BINARY);
    // Also test Otsus binarization
    cv::adaptiveThreshold(processed_img, processed_img, 255,cv::ADAPTIVE_THRESH_MEAN_C, cv::THRESH_BINARY_INV,3,3);
    
    // string filename = samples::findFile(names[i]);

    return processed_img;
    
}

string type2str(int type) {
  string r;

  uchar depth = type & CV_MAT_DEPTH_MASK;
  uchar chans = 1 + (type >> CV_CN_SHIFT);

  switch ( depth ) {
    case CV_8U:  r = "8U"; break;
    case CV_8S:  r = "8S"; break;
    case CV_16U: r = "16U"; break;
    case CV_16S: r = "16S"; break;
    case CV_32S: r = "32S"; break;
    case CV_32F: r = "32F"; break;
    case CV_64F: r = "64F"; break;
    default:     r = "User"; break;
  }

  r += "C";
  r += (chans+'0');

  return r;
}

void find_cards(cv::Mat image){

    /* Finds all card-sized contours in a thresholded camera image.
    Returns the number of cards, and a list of card contours sorted
    from largest to smallest. */
    std::vector<std::vector<cv::Point>> contours;
    std::vector<cv::Vec4i> hierarchy;
    cv::findContours(image, contours, hierarchy, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE );
    // If there are no contours, do nothing
    if (contours.size() == 0){
        return;
    }    
    // std::cout << contours.size() << std::endl;

    /* Draw the Contours */
    // Mat drawing = Mat::zeros( image.size(), CV_8UC3 );
    // for( size_t i = 0; i< contours.size(); i++ )
    // {
    //     Scalar color = Scalar( 255, 255-i, 255-i );
    //     drawContours( drawing, contours, (int)i, color, 2, LINE_8, hierarchy, 0 );

    // }
    // cv::imshow("Contours", drawing);

    // Determine which of the contours are cards by applying the
    // following criteria: 1) Smaller area than the maximum card size,
    // 2), bigger area than the minimum card size, 3) have no parents,
    // and 4) have four corners
    double area, perimeter;
    std::vector<cv::Point> approx;
    int num_of_cards = 0;
    for( size_t i = 0; i< contours.size(); i++ ){
        area = std::abs(cv::contourArea(contours[i]));
        perimeter = cv::arcLength(contours[i], true);
        // approximate contour with accuracy proportional
        // to the contour perimeter
        cv::approxPolyDP(contours[i], approx, 0.01*perimeter, true);

        // square contours should have 4 vertices after approximation
        // relatively large area (to filter out noisy contours)
        // and be convex.
        // Note: absolute value of an area is used because
        // area may be positive or negative - in accordance with the
        // contour orientation
        if( approx.size() == 4 && area < CARD_MAX_AREA && \
        area > CARD_MIN_AREA && cv::isContourConvex(approx) && hierarchy[i][3] == -1){
            // contour_is_card[i] = 1;
            num_of_cards ++;
        }
    }
    std::cout << num_of_cards << std::endl;

    // for i in range(len(cnts_sort)):
    //     peri = cv2.arcLength(cnts_sort[i],True)
    //     approx = cv2.approxPolyDP(cnts_sort[i],0.01*peri,True)
        
    //     if ((size < CARD_MAX_AREA) and (size > CARD_MIN_AREA)
    //         and (hier_sort[i][3] == -1) and (len(approx) == 4)):
    //         cnt_is_card[i] = 1

    // return cnts_sort, cnt_is_card
}

class CameraCallback : public CallbackLinker{
    virtual void passFrame(Mat nextFrame){
        cv::Mat processed_image = preprocess_image(nextFrame);
        find_cards(processed_image);
        /* Need to Show frame after find_cards()*/
        /* imshow converts image to 3-channel */
        /* find_cards() requires single monochrome channel */
        cv::imshow("Frame", processed_image);
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

