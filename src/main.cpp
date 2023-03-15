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
#define CARD_MIN_AREA 10000
// Width and height of card corner, where rank and suit are
#define FLATTENED_WIDTH 230
#define FLATTENED_HEIGHT 300
// Dimensions of rank train images
#define RANK_WIDTH 70
#define RANK_HEIGHT 125
RNG rng(12345);


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

cv::Mat preprocess_image(cv::Mat image){

    // Returns a grayed, blurred, and adaptively thresholded camera image.
    cv::Mat processed_img;
    cv::cvtColor(image, processed_img, cv::COLOR_BGR2GRAY);
    cv::GaussianBlur(processed_img, processed_img,cv::Size(5,5),0);
    /* Canny Edge Detection filter seems to improve detection of cards with Red coloured suits*/
    // cv::Canny(processed_img, processed_img, 100, 200);
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
    // cv::threshold(processed_img, processed_img, 0, 255, cv::THRESH_OTSU);
    // Also test Otsus binarization

    /* Adaptive threshold provides image in the same format as
     * the matching template, thus reducing the need to re-format
     * in later steps */
    cv::adaptiveThreshold(processed_img, processed_img, 255,cv::ADAPTIVE_THRESH_MEAN_C, cv::THRESH_BINARY_INV,3,3);
    
    // string filename = samples::findFile(names[i]);

    return processed_img;
    
}

struct Card_params{
    int err = 0;
    int num_of_cards = 0;
    std::vector<int> contour_is_card_idx;
    std::vector<std::vector<cv::Point>> contours;
    std::vector<std::vector<cv::Point>> card_approxs;
    std::vector<double> card_peris;
    std::vector<cv::RotatedRect> rotatedbox;
    std::vector<std::vector<cv::Point2f>> rotatedbox_pts; 

};

struct Card_params find_cards(cv::Mat image){

    /* Finds all card-sized contours in a thresholded camera image.
    Returns the number of cards, and a list of card contours sorted
    from largest to smallest. */
    Card_params Card_params;
    std::vector<std::vector<cv::Point>> contours;
    std::vector<cv::Vec4i> hierarchy;
    cv::findContours(image, contours, hierarchy, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE );
    size_t num_cntrs = contours.size();
    Card_params.contours = contours;

    // If there are no contours, do nothing
    if (num_cntrs == 0){
        Card_params.err = 1;
        return Card_params;
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
    double area, perimeter, area_approx;
    std::vector<cv::Point> approx;

    for( int i = 0; i < num_cntrs; i++ ){
        area = std::abs(cv::contourArea(contours[i]));
        perimeter = cv::arcLength(contours[i], true);
        // approximate contour with accuracy proportional
        // to the contour perimeter
        cv::approxPolyDP(contours[i], approx, 0.01*perimeter, true);
        area_approx = std::abs(cv::contourArea(approx));

        // square contours should have 4 vertices after approximation
        // relatively large area (to filter out noisy contours)
        // be convex and not have parent contours.
        // Note: absolute value of an area is used because
        // area may be positive or negative - in accordance with the
        // contour orientation
        if( approx.size() == 4 && area_approx < CARD_MAX_AREA && \
        area_approx > CARD_MIN_AREA && cv::isContourConvex(approx) && hierarchy[i][3] == -1){
            
            /* Store the parameters and the index position of the contour that is a card */

            /* Increment the number of cards variable */
            Card_params.num_of_cards ++;

            /* Store Index position */
            Card_params.contour_is_card_idx.push_back(i);

            /* Store Card Approximations*/
            Card_params.card_approxs.push_back(approx); // approx has 4 pairs of (x,y) coordinates

            /* Store Card Perimeters*/
            Card_params.card_peris.push_back(perimeter);

            std::vector<cv::Point2f> box_pts(4); 
            cv::RotatedRect boundingBox = cv::minAreaRect(contours[i]);
            boundingBox.points(box_pts.data());
            Card_params.rotatedbox.push_back(boundingBox);
            Card_params.rotatedbox_pts.push_back(box_pts);

        }
    }

    return Card_params;
}

class Query_card
{
    /* Structure to store information about single card in the camera image.*/
    public:
        Query_card(){};
        ~Query_card(){};
        std::vector<std::vector<cv::Point>> contours; /* Contour of card */
        cv::Size card_size; /* Card dimensions */
        std::vector<cv::Point> corner_pts; /* Card corner points */
        cv::Point_<int> centre_pts; /* Card centre points */
        // self.warp = [] # 200x300, flattened, grayed, blurred image
        // self.rank_img = [] # Thresholded, sized image of card's rank
        // self.suit_img = [] # Thresholded, sized image of card's suit
        // self.best_rank_match = "Unknown" # Best matched rank
        // self.best_suit_match = "Unknown" # Best matched suit
        // self.rank_diff = 0 # Difference between rank image and best matched train rank image
        // self.suit_diff = 0 # Difference between suit image and best matched train suit image
        cv::RotatedRect rotatedbox;

};

cv::Mat flatten_card(Query_card qCard, cv::Mat image){
    /* If card is placed VERTICALLY, then card Rank is 
     * in the [0] and [2] corner points */
    /* If card is placed HORIZONTALLY, then card Rank is 
     * in the [1] and [3] corner points */
    /* If card is placed at an angle, then card Rank is 
     * in the [1] and [3] or [0] and [2] corner points */

    std::vector< cv::Point2f> roi_corners(4);
    float width = (float)qCard.card_size.width;
    float height = (float)qCard.card_size.height;
    cv::Point tl;
    int sum_min_idx;
    std::vector<int> sum;
    std::vector<int>::iterator result;

    /* Sum the coordinates of all 4 corners 
     * The top left corner will have the smallest sum */
    for (int i=0; i < qCard.corner_pts.size(); i++){
        sum.push_back(qCard.corner_pts[i].x + qCard.corner_pts[i].y); 
    }
    /* Find the minimum value */
    result = std::min_element(sum.begin(), sum.end());
    /* Find the index of the minimum value */
    sum_min_idx = std::distance(sum.begin(), result);


    tl = qCard.corner_pts[sum_min_idx];

    /* For VERTICAL cards, width < height, and ratio
     * height/width = ~ 1.4 */
    if (height >= 1.3 * width){
        if (tl == qCard.corner_pts[0]){
            roi_corners[0] = qCard.corner_pts[0]; // T-L
            roi_corners[1] = qCard.corner_pts[1]; // B-L
            roi_corners[2] = qCard.corner_pts[2]; // B-R
            roi_corners[3] = qCard.corner_pts[3]; // T-R
        }
        else{
            roi_corners[0] = qCard.corner_pts[1]; // T-L
            roi_corners[1] = qCard.corner_pts[2]; // B-L
            roi_corners[2] = qCard.corner_pts[3]; // B-R
            roi_corners[3] = qCard.corner_pts[0]; // T-R
        }

        // std::cout << "Card is VERTICAL" << endl;
    }
    
    /* For HORIZONTAL cards, width > height, and ratio
     * height/width = ~ 1/1.3 = 0.77 */
    
    if (height <= 0.77 * width){
        if (tl == qCard.corner_pts[0]){
            roi_corners[0] = qCard.corner_pts[3]; // T-L
            roi_corners[1] = qCard.corner_pts[0]; // B-L
            roi_corners[2] = qCard.corner_pts[1]; // B-R
            roi_corners[3] = qCard.corner_pts[2]; // T-R
        }
        else{
            roi_corners[0] = qCard.corner_pts[0]; // T-L
            roi_corners[1] = qCard.corner_pts[1]; // B-L
            roi_corners[2] = qCard.corner_pts[2]; // B-R
            roi_corners[3] = qCard.corner_pts[3]; // T-R
        }
        // std::cout << "Card is HORIZONTAL" << endl;
    }

    /* For cards at an angle, height/width < 1.4 and height/width > 0.72
     * Also find which direction card is tilted */
    if (height > 0.77 * width && height < 1.3 * width){
        /* Card tilted to the right*/
        if (qCard.corner_pts[1].y > qCard.corner_pts[3].y
        && qCard.rotatedbox.angle > 5 && qCard.rotatedbox.angle < 40){
            roi_corners[0] = qCard.corner_pts[0]; // T-L
            roi_corners[1] = qCard.corner_pts[1]; // B-L
            roi_corners[2] = qCard.corner_pts[2]; // B-R
            roi_corners[3] = qCard.corner_pts[3]; // T-R
            // std::cout << "Card is TILTED RIGHT" << endl;
        }
        /* Card tilted to the left*/
        if (qCard.corner_pts[1].y < qCard.corner_pts[3].y 
        && qCard.rotatedbox.angle < 85 && qCard.rotatedbox.angle > 40){
            roi_corners[0] = qCard.corner_pts[1]; // T-L
            roi_corners[1] = qCard.corner_pts[2]; // B-L
            roi_corners[2] = qCard.corner_pts[3]; // B-R
            roi_corners[3] = qCard.corner_pts[0]; // T-R
            // std::cout << "Card is TILTED LEFT" << endl;
        }
    }


    // Create destination array, calculate perspective transform matrix, and warp card image
    std::vector<cv::Point2f> dst = {
        cv::Point2f(0, 0), // T-L
        cv::Point2f((float)0, (float)(FLATTENED_HEIGHT-1)), // B-L
        cv::Point2f((float)(FLATTENED_WIDTH-1), (float)(FLATTENED_HEIGHT-1)), // B-R
        cv::Point2f((float)(FLATTENED_WIDTH-1), 0) // T-R
        };

    cv::Mat M = cv::getPerspectiveTransform(roi_corners, dst);
    cv::Mat wrapped;
    cv::warpPerspective(image, wrapped, M, cv::Size(FLATTENED_WIDTH, FLATTENED_HEIGHT));

    return wrapped;

}

cv::Mat preprocess_card(cv::Mat image, struct Card_params Card_params)
{
    /* Uses contour to find information about the query card. Isolates rank
    and suit images from the card.*/

    // Initialize new Query_card object
    Query_card qCard;

    /* Check if Card_params is filled before procceeding */
    if (Card_params.num_of_cards == 0 || Card_params.err || Card_params.card_approxs.empty()){
        return image;
    }
    qCard.contours = Card_params.contours;
    qCard.rotatedbox = Card_params.rotatedbox[0];
    // Corner Points of a single card
    /* Corner points are stored with [0] position 
     * as the point closest to the top of the Frame (min y).
     * The remaining points are then populated in counter-clockwise
     * direction. */
    qCard.corner_pts = Card_params.card_approxs.front();

    // Find width and height of card's bounding rectangle
    cv::Rect boundingBox = cv::boundingRect(qCard.corner_pts);
    qCard.card_size.height = boundingBox.height;
    qCard.card_size.width = boundingBox.width;

    // Find center point of card by taking x and y average of the four corners.
    size_t num_corners = qCard.corner_pts.size();
    for (int k = 0; k < num_corners; k++){
        qCard.centre_pts.x += qCard.corner_pts[k].x;
        qCard.centre_pts.y += qCard.corner_pts[k].y;
    }
    qCard.centre_pts.x /= (int)num_corners;
    qCard.centre_pts.y /= (int)num_corners;

    cv::Mat flat_card = flatten_card(qCard, image);

    /* Draw the Box */
    cv::cvtColor(image, image, cv::COLOR_GRAY2BGR);
    // cv::rectangle(image, boundingBox.boundingRect(), Scalar( 0, 255, 0), 2);
    for (int i = 0; i < 4; i++){
        cv::line(image, Card_params.rotatedbox_pts[0][i], Card_params.rotatedbox_pts[0][(i+1)%4], Scalar(0,255,0), 2);
    }
    // std::cout << "Card angle: " << Card_params.rotatedbox[0].angle << endl;
    // Warp card into 200x300 flattened image using perspective transform
    // cv::imshow("Flattened", flat_card);

    /* The corner division by 7 is good for the wider cards*/
    /* Corner dimension relative to card width-height to account
     * for varying camera vertical height (distance to card) */
    cv::Mat Qcorner = flat_card(cv::Rect(0,0, FLATTENED_WIDTH/7, FLATTENED_HEIGHT/4));


    /* Resize up by factor of 4 */
    cv::resize(Qcorner, Qcorner, cv::Size(), 4, 4, cv::INTER_LINEAR);
    /* Threshold the upsized image. The rank and suit will look bolder and clearer */
    cv::threshold(Qcorner, Qcorner, 40, 255, cv::THRESH_BINARY);

    std::vector<std::vector<cv::Point>> rank_contours, suit_contours;
    std::vector<cv::Vec4i> rank_hierarchy, suit_hierarchy;
    // Split in to top and bottom half (top shows rank, bottom shows suit)
    int vert_offset = 20; // Crop 20 rows further down to better isolate the rank and suit
    int hor_offset = 5; // Crop 5 columns further down to better isolate the rank and suit
    cv::Mat rank = Qcorner(cv::Rect(hor_offset, vert_offset, 4*(FLATTENED_WIDTH/7) - hor_offset, FLATTENED_HEIGHT/2));
    cv::Mat suit = Qcorner(cv::Rect(hor_offset, vert_offset + FLATTENED_HEIGHT/2, 4*(FLATTENED_WIDTH/7) - hor_offset, (FLATTENED_HEIGHT/2) - vert_offset));

    cv::findContours(rank, rank_contours, rank_hierarchy, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE);
    cv::findContours(suit, suit_contours, suit_hierarchy, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE);

    /* Isolated rank and suit images will have additional artifacts present
     * due to non-accurate cropping, or including symbols too close to the rank-suit */
    
    /* Sort rank contours by area, largest placed first */
    std::sort(rank_contours.begin(), rank_contours.end(), [](const std::vector<Point>& c1, const std::vector<Point>& c2)
    {
        return cv::contourArea(c1, false) > cv::contourArea(c2, false);
    });

    /* Sort suit contours by area, largest placed first*/
    std::sort(suit_contours.begin(), suit_contours.end(), [](const std::vector<Point>& c1, const std::vector<Point>& c2)
    {
        return cv::contourArea(c1, false) > cv::contourArea(c2, false);
    });

    /* Create a bounding box around the largest contours and upsize it to the dimensions 
     * of the reference images that will be used for matching */
    if(rank_contours.size()){
        cv::Rect rank_box = cv::boundingRect(rank_contours[0]);
        cv::Mat rank_roi = rank(rank_box);
        cv::resize(rank_roi, rank_roi, cv::Size(RANK_WIDTH, RANK_HEIGHT), 1, 1, cv::INTER_LINEAR);
        cv::imshow("Rank ROI", rank_roi);
    }
    if(suit_contours.size()){
        cv::Rect suit_box = cv::boundingRect(suit_contours[0]);
        cv::Mat suit_roi = suit(suit_box);
        cv::resize(suit_roi, suit_roi, cv::Size(RANK_WIDTH, RANK_HEIGHT), 1, 1, cv::INTER_LINEAR);
        cv::imshow("Suit ROI", suit_roi);
    }

    return image;
}

class CameraCallback : public CallbackLinker{
    virtual void passFrame(Mat nextFrame){
        cv::Mat processed_image = preprocess_image(nextFrame);
        Card_params card_params = find_cards(processed_image);
        processed_image = preprocess_card(processed_image, card_params);
        /* Need to Show frame after find_cards()*/
        /* imshow converts image to 3-channel */
        /* find_cards() requires single monochrome channel */
        cv::imshow("Frame", processed_image);
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

