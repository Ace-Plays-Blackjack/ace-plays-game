/*
Copyright 2023 Georgios Titas
Copyright 2023 Alexander Douglas
Copyright 2023 Jijo Varghese
Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the “Software”), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#ifndef CARDS_H
#define CARDS_H

#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <vector>
#include <iostream>

/* Defined based on specific Hardware setup 
such as camera height from table/cards */
#define CARD_MAX_AREA 120000
#define CARD_MIN_AREA 10000
/* Width and height of card corner, where rank and suit are */
#define FLATTENED_WIDTH 230
#define FLATTENED_HEIGHT 300
/* Dimensions of rank train images */
#define RANK_WIDTH 70
#define RANK_HEIGHT 125

/**
 * @brief A Blackjack enum decisions
 * to determine the next Optimal Play
 * 
 */
enum decisions {
    HIT,
    STAND,
    SPLIT,
    DOUBLE,
    UNKNOWN,
    LOSE,
    STOP
};

/**
 * @brief Structure holding the parameters of all detected
 * card shapes in a frame, such as their position in the frame,
 * the isolated Rank RoIs and others.
 * 
 */
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

/**
 * @brief Utility structure used by the AcePlays repo
 * for passing necessary information down the processing
 * pipeline, from one class to the next
 * 
 */
struct AcePlaysUtils{
    cv::Mat nextFrame;
    Card_params cardParams;
    decisions blackjackDecision;
    int dealercard; 
    std::vector<int> playercards;
};

/**
 * @brief Structure to store information about single card in the camera image.
 * 
 */
struct qCard
{
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
