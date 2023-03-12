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

    // cv::threshold(processed_img, processed_img, thresh_level, 255, cv::THRESH_BINARY);
    cv::threshold(processed_img, processed_img, 0, 255, cv::THRESH_OTSU);
    // Also test Otsus binarization
    // cv::adaptiveThreshold(processed_img, processed_img, 255,cv::ADAPTIVE_THRESH_MEAN_C, cv::THRESH_BINARY_INV,3,3);
    
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

};

cv::Mat flatten_card(Query_card qCard, cv::Mat image){
    /* If card is placed VERTICALLY, then card Rank is 
     * in the [0] and [2] corner points */
    /* If card is placed HORIZONTALLY, then card Rank is 
     * in the [1] and [3] corner points */
    /* If card is placed at an angle, then card Rank is 
     * in the [1] and [3] or [0] and [2] corner points */

    std::vector< cv::Point2f> roi_corners(4);
    std::vector< cv::Point2f> dst_corners(4);
    float width = (float)qCard.card_size.width;
    float height = (float)qCard.card_size.height;
    
    /* For VERTICAL cards, width < height, and ratio
     * height/width = ~ 1.4 */
    if (height >= 1.4 * width){
        roi_corners[0] = qCard.corner_pts[0]; // T-L
        roi_corners[1] = qCard.corner_pts[1]; // B-L
        roi_corners[2] = qCard.corner_pts[2]; // B-R
        roi_corners[3] = qCard.corner_pts[3]; // T-R
        std::cout << "Card is VERTICAL" << endl;
    }
    
    /* For HORIZONTAL cards, width > height, and ratio
     * height/width = ~ 1/1.4 = 0.72 */
    
    if (height <= 0.72 * width){
        roi_corners[0] = qCard.corner_pts[3]; // T-L
        roi_corners[1] = qCard.corner_pts[0]; // B-L
        roi_corners[2] = qCard.corner_pts[1]; // B-R
        roi_corners[3] = qCard.corner_pts[2]; // T-R
        std::cout << "Card is HORIZONTAL" << endl;
    }

    /* For cards at an angle, height/width < 1.4 and height/width > 0.72
     * Also find which direction card is tilted */
    if (height > 0.72 * width && height < 1.4 * width){
        /* Card tilted to the right*/
        if (qCard.corner_pts[1].y <= qCard.corner_pts[3].y ){
            roi_corners[0] = qCard.corner_pts[0]; // T-L
            roi_corners[1] = qCard.corner_pts[1]; // B-L
            roi_corners[2] = qCard.corner_pts[2]; // B-R
            roi_corners[3] = qCard.corner_pts[3]; // T-R
            std::cout << "Card is TILTED RIGHT" << endl;
        }
        /* Card tilted to the left*/
        if (qCard.corner_pts[1].y >= qCard.corner_pts[3].y ){
            roi_corners[0] = qCard.corner_pts[1]; // T-L
            roi_corners[1] = qCard.corner_pts[2]; // B-L
            roi_corners[2] = qCard.corner_pts[3]; // B-R
            roi_corners[3] = qCard.corner_pts[0]; // T-R
            std::cout << "Card is TILTED LEFT" << endl;
        }
    }


    int maxWidth = 200;
    int maxHeight = 300;

    // Create destination array, calculate perspective transform matrix, and warp card image
    std::vector<cv::Point2f> dst = {
        cv::Point2f(0, 0), // T-L
        cv::Point2f((float)0, (float)(maxHeight-1)), // B-L
        cv::Point2f((float)(maxWidth-1), (float)(maxHeight-1)), // B-R
        cv::Point2f((float)(maxWidth-1), 0) // T-R
        };

    cv::Mat M = cv::getPerspectiveTransform(roi_corners, dst);
    cv::warpPerspective(image, image, M, cv::Size(maxWidth, maxHeight));
    // warp = cv2.cvtColor(warp,cv2.COLOR_BGR2GRAY)
    // cv::Mat nothing;
    return image;

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

    /* Draw the Box */
    cv::cvtColor(image, image, cv::COLOR_GRAY2BGR);
    cv::rectangle(image, boundingBox, Scalar( 0, 255, 0), 2);

    cv::imshow("Flattened", flatten_card(qCard, image));

    // Warp card into 200x300 flattened image using perspective transform
    // qCard.warp = cv: (image, pts, w, h)

    // Grab corner of warped card image and do a 4x zoom
    // Qcorner = qCard.warp[0:CORNER_HEIGHT, 0:CORNER_WIDTH]
    // Qcorner_zoom = cv2.resize(Qcorner, (0,0), fx=4, fy=4)

    // # Sample known white pixel intensity to determine good threshold level
    // white_level = Qcorner_zoom[15,int((CORNER_WIDTH*4)/2)]
    // thresh_level = white_level - CARD_THRESH
    // if (thresh_level <= 0):
    //     thresh_level = 1
    // retval, query_thresh = cv2.threshold(Qcorner_zoom, thresh_level, 255, cv2. THRESH_BINARY_INV)
    
    // # Split in to top and bottom half (top shows rank, bottom shows suit)
    // Qrank = query_thresh[20:185, 0:128]
    // Qsuit = query_thresh[186:336, 0:128]

    // # Find rank contour and bounding rectangle, isolate and find largest contour
    // dummy, Qrank_cnts, hier = cv2.findContours(Qrank, cv2.RETR_TREE,cv2.CHAIN_APPROX_SIMPLE)
    // Qrank_cnts = sorted(Qrank_cnts, key=cv2.contourArea,reverse=True)

    // # Find bounding rectangle for largest contour, use it to resize query rank
    // # image to match dimensions of the train rank image
    // if len(Qrank_cnts) != 0:
    //     x1,y1,w1,h1 = cv2.boundingRect(Qrank_cnts[0])
    //     Qrank_roi = Qrank[y1:y1+h1, x1:x1+w1]
    //     Qrank_sized = cv2.resize(Qrank_roi, (RANK_WIDTH,RANK_HEIGHT), 0, 0)
    //     qCard.rank_img = Qrank_sized

    // # Find suit contour and bounding rectangle, isolate and find largest contour
    // dummy, Qsuit_cnts, hier = cv2.findContours(Qsuit, cv2.RETR_TREE,cv2.CHAIN_APPROX_SIMPLE)
    // Qsuit_cnts = sorted(Qsuit_cnts, key=cv2.contourArea,reverse=True)
    
    // # Find bounding rectangle for largest contour, use it to resize query suit
    // # image to match dimensions of the train suit image
    // if len(Qsuit_cnts) != 0:
    //     x2,y2,w2,h2 = cv2.boundingRect(Qsuit_cnts[0])
    //     Qsuit_roi = Qsuit[y2:y2+h2, x2:x2+w2]
    //     Qsuit_sized = cv2.resize(Qsuit_roi, (SUIT_WIDTH, SUIT_HEIGHT), 0, 0)
    //     qCard.suit_img = Qsuit_sized

    // return qCard
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

