#ifndef CARDS_H
#define CARDS_H

#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <vector>
#include <iostream>

#define BKG_ADAPTIVE_THRESH 50
#define CARD_MAX_AREA 120000
#define CARD_MIN_AREA 10000
// Width and height of card corner, where rank and suit are
#define FLATTENED_WIDTH 230
#define FLATTENED_HEIGHT 300
// Dimensions of rank train images
#define RANK_WIDTH 70
#define RANK_HEIGHT 125


enum decisions {
    HIT,
    STAND,
    SPLIT,
    DOUBLE,
    UNKNOWN,
    LOSE,
    STOP
};

struct Card_params
{
    int err = 0;
    int num_of_cards = 0;
    cv::Mat currentFrame;
    std::vector<int> contour_is_card_idx;
    /* Holds all contours of single frame */
    std::vector<std::vector<cv::Point>> contours;
    /* Card corner points */
    std::vector<std::vector<cv::Point>> card_approxs;
    /* Card centre points */
    std::vector<cv::Point_<int>> centre_pts;
    /* Card dimensions */
    std::vector<cv::Size> card_size; 
    /* Card rotated bounding box */
    std::vector<cv::RotatedRect> rotatedbox;
    std::vector<std::vector<cv::Point2f>> rotatedbox_pts; 
    /* Vector of images to hold all detected Ranks */
    std::vector<cv::Mat> rank_rois;
    /* Hold detect card names */
    std::vector<cv::String> card_names;
};

struct AcePlaysUtils{
    cv::Mat nextFrame;
    Card_params cardParams;
    decisions blackjackDecision;
};

struct qCard
{
    /* Structure to store information about single card in the camera image.*/
    /* Card dimensions */
    cv::Size card_size;
    /* Card centre points */
    cv::Point_<int> centre_pts;
    /* Card corner points */
    std::vector<cv::Point> corner_pts;
    /* Card rotated bounding box */
    cv::RotatedRect rotatedbox;
};

#endif /* CARDS_H */
