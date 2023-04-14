/*
Copyright 2023 Georgios Titas
Copyright 2023 Alexander Douglas
Copyright 2023 Jijo Varghese
Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the “Software”), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#include "DetectCard.h"


/**
 * @brief Preprocesses a frame. Frame is turned to grayscale and blurred
 * before applying thresholding to turn into monochrome.
 * 
 * @param image the current frame to be processsed
 * @return cv::Mat processed frame
 */
cv::Mat DetectCard::preprocess_image(cv::Mat &image){

    /* Returns a grayed, blurred, and adaptively thresholded camera image */
    cv::Mat processed_img;
    try{
        /* cvtColor may crash if image is empty */
        cv::cvtColor(image, processed_img, cv::COLOR_BGR2GRAY);
    }
    catch (cv::Exception& e)
    {
        const char* err_msg = e.what();
        std::cout << "Exception caught (DetectCard::preprocess_image): cv::cvtColor:\n" << err_msg << std::endl;
        err_frame = true;
        return image;
    }
    try{
        /* GaussianBlur may crash if image is empty */
        cv::GaussianBlur(processed_img, processed_img,cv::Size(5,5),0);
    }
    catch (cv::Exception& e)
    {
        const char* err_msg = e.what();
        std::cout << "Exception caught (DetectCard::preprocess_image): cv::GaussianBlur:\n" << err_msg << std::endl;
        err_frame = true;
        return image;
    }

    /* Adaptive threshold provides image in the same format as
     * the matching template, thus reducing the need to re-format
     * in later steps and the processing workload */
    cv::adaptiveThreshold(processed_img, processed_img, 255,cv::ADAPTIVE_THRESH_MEAN_C, cv::THRESH_BINARY_INV,3,3);

    return processed_img;
}

/**
 * @brief Find card in current frame by checking for rectangular
 * shaped objects in the frame.
 * 
 * @param image the frame to process
 * @return struct Card_params contains contours, bounding boxes and other parameters
 * for all cards, required in later steps of the pipeline
 */
Card_params DetectCard::find_cards(cv::Mat &image){

    /* Finds all card-sized contours in a thresholded camera image.
     * Returns the number of cards, and a list of card contours */
    Card_params Card_params;
    std::vector<std::vector<cv::Point>> contours;
    std::vector<cv::Vec4i> hierarchy;

    /* Find the contours */
    cv::findContours(image, contours, hierarchy, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE );
    size_t num_cntrs = contours.size();

    /* Store contours */
    Card_params.contours = contours;

    /* If there are no contours, do nothing */
    if (num_cntrs == 0){
        Card_params.err = 1;
        return Card_params;
    }    

    /* Contours that are cards will have an area smaller 
     * between the max and min area thresholds, are the outer-most
     * contour (no parent contours) and have four corners */
    double area, perimeter, area_approx;
    std::vector<cv::Point> approx;

    for( int i = 0; i < num_cntrs; i++ ){
        area = std::abs(cv::contourArea(contours[i]));
        perimeter = cv::arcLength(contours[i], true);
        /* Approximate contour with accuracy proportional to the contour perimeter */
        cv::approxPolyDP(contours[i], approx, 0.01*perimeter, true);
        area_approx = std::abs(cv::contourArea(approx));

        /* Check condition for contour being a card. 
         * Additional condition is the contour being convex shape */
        if( approx.size() == 4 && area_approx < CARD_MAX_AREA && \
        area_approx > CARD_MIN_AREA && cv::isContourConvex(approx) && hierarchy[i][3] == -1){
            
            /* Store the parameters and the index position of the contour that is a card */

            /* Increment the number of cards variable */
            Card_params.num_of_cards ++;

            /* Store Index position */
            Card_params.contour_is_card_idx.push_back(i);

            /* Store Card Approximations*/
            /* Corner points are stored with [0] position 
             * as the point closest to the top of the Frame (min y).
             * The remaining points are then populated in counter-clockwise
             * direction. */
            Card_params.card_approxs.push_back(approx); // approx has 4 pairs of (x,y) coordinates

            /* Create rotated bounding boxes around cards */
            std::vector<cv::Point2f> box_pts(4); 
            cv::RotatedRect boundingBox = cv::minAreaRect(contours[i]);
            boundingBox.points(box_pts.data());

            /* Store each bounding box and its points */
            Card_params.rotatedbox.push_back(boundingBox);
            Card_params.rotatedbox_pts.push_back(box_pts);
        }
    }

    return Card_params;
}

/**
 * @brief Flatten the detected card bounding box
 * by applying a warpPrespective opencv method.
 * Aids in counteracting optical distorion for objects
 * in the corners of the camera FoV.
 * 
 * @param qCard structure to store information about single card in the camera image
 * @param image the current frame
 * @return cv::Mat flattened card
 */
cv::Mat DetectCard::flatten_card(qCard &qCard, cv::Mat &image){
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
    sum_min_idx = (int)std::distance(sum.begin(), result);


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
        }
        /* Card tilted to the left*/
        if (qCard.corner_pts[1].y < qCard.corner_pts[3].y 
        && qCard.rotatedbox.angle < 85 && qCard.rotatedbox.angle > 40){
            roi_corners[0] = qCard.corner_pts[1]; // T-L
            roi_corners[1] = qCard.corner_pts[2]; // B-L
            roi_corners[2] = qCard.corner_pts[3]; // B-R
            roi_corners[3] = qCard.corner_pts[0]; // T-R
        }
    }


    /* Create destination array, calculate perspective transform matrix, and warp card image */
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

/**
 * @brief Extract the Cards Ranks as Regions of Interest.
 * Isolates the ranks as vector of cv::Mat.
 * 
 * @param Card_params structure holding all parameters of
 * card-shaped objects
 * @param image the current frame to be processed
 */
void DetectCard::preprocess_cards(Card_params &Card_params, cv::Mat &image)
{
    /* Uses contour to find information about the query card. Isolates rank
    and suit images from the card.*/

    /* Vector of images to hold all detected Ranks */
    std::vector<cv::Mat> rank_rois;

    /* Check if Card_params is filled before procceeding */
    if (Card_params.num_of_cards == 0 || Card_params.err || Card_params.card_approxs.empty()){
        return;
    }

    for(int k = 0; k < Card_params.num_of_cards; k++){
        qCard qCard;
        qCard.corner_pts = Card_params.card_approxs[k];

        /* Find width and height of card's bounding rectangle */
        cv::Rect boundingBox = cv::boundingRect(qCard.corner_pts);
        qCard.card_size.height = boundingBox.height;
        qCard.card_size.width = boundingBox.width;

        /* Find center point of card by taking x and y average of the four corners */
        size_t num_corners = qCard.corner_pts.size();
        for (int i = 0; i < num_corners; i++){
            qCard.centre_pts.x += qCard.corner_pts[i].x;
            qCard.centre_pts.y += qCard.corner_pts[i].y;
        }
        qCard.centre_pts.x /= (int)num_corners;
        qCard.centre_pts.y /= (int)num_corners;

        /* Store calculated card parameters in Card_params */
        Card_params.card_size.push_back(qCard.card_size);
        Card_params.centre_pts.push_back(qCard.centre_pts);
        Card_params.rotatedbox.push_back(qCard.rotatedbox);

        /* Flatten the card to fix for wrapped perspective */
        cv::Mat flat_card = flatten_card(qCard, image);

        /* The corner division by 7 is good for the wider cards*/
        /* Corner dimension relative to card width-height to account
        * for varying camera vertical height (distance to card) */
        cv::Mat Qcorner = flat_card(cv::Rect(0,0, FLATTENED_WIDTH/7, FLATTENED_HEIGHT/4));


        /* Resize up by factor of 4 */
        cv::resize(Qcorner, Qcorner, cv::Size(), 4, 4, cv::INTER_LINEAR);
        /* Threshold the upsized image. The rank and suit will look bolder and clearer with a small threshold */
        cv::threshold(Qcorner, Qcorner, 5, 255, cv::THRESH_BINARY);

        std::vector<std::vector<cv::Point>> rank_contours, suit_contours;
        std::vector<cv::Vec4i> rank_hierarchy, suit_hierarchy;
        /* Split in to top and bottom half (top shows rank, bottom shows suit) */
        int vert_offset = 20; // Crop 20 rows further down to better isolate the rank and suit
        int hor_offset = 5; // Crop 5 columns further down to better isolate the rank and suit
        cv::Mat rank = Qcorner(cv::Rect(hor_offset, vert_offset, 4*(FLATTENED_WIDTH/7) - hor_offset, FLATTENED_HEIGHT/2));
        cv::Mat suit = Qcorner(cv::Rect(hor_offset, vert_offset + FLATTENED_HEIGHT/2, 4*(FLATTENED_WIDTH/7) - hor_offset, (FLATTENED_HEIGHT/2) - vert_offset));

        cv::findContours(rank, rank_contours, rank_hierarchy, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE);
        cv::findContours(suit, suit_contours, suit_hierarchy, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE);

        /* Isolated rank and suit images will have additional artifacts present
        * due to non-accurate cropping, or including symbols too close to the rank-suit */
        
        /* Sort rank contours by area, largest placed first */
        std::sort(rank_contours.begin(), rank_contours.end(), [](const std::vector<cv::Point>& c1, const std::vector<cv::Point>& c2)
        {
            return cv::contourArea(c1, false) > cv::contourArea(c2, false);
        });

        /* Sort suit contours by area, largest placed first*/
        std::sort(suit_contours.begin(), suit_contours.end(), [](const std::vector<cv::Point>& c1, const std::vector<cv::Point>& c2)
        {
            return cv::contourArea(c1, false) > cv::contourArea(c2, false);
        });

        /* Create a bounding box around the largest contour and upsize it to the dimensions 
        * of the reference images that will be used for matching */
        cv::Mat rank_roi, suit_roi;
        if(rank_contours.size()){
            /* Isolate the largest contour */
            cv::Rect rank_box = cv::boundingRect(rank_contours[0]);
            rank_roi = rank(rank_box);
            cv::resize(rank_roi, rank_roi, cv::Size(RANK_WIDTH, RANK_HEIGHT), 1, 1, cv::INTER_LINEAR);
            rank_rois.push_back(rank_roi);
        }
        if(suit_contours.size()){
            /* Isolate the largest contour */
            cv::Rect suit_box = cv::boundingRect(suit_contours[0]);
            suit_roi = suit(suit_box);
            cv::resize(suit_roi, suit_roi, cv::Size(RANK_WIDTH, RANK_HEIGHT), 1, 1, cv::INTER_LINEAR);
        }
    }

    /* Store the Rank ROIs to be passed on the template matching method */
    Card_params.rank_rois = rank_rois;
}

/**
 * @brief Template matching method is responsible for determining
 * the card Rank. In this release, the matching method
 * is a simple absolute difference of the Rank ROI against a 
 * template image, however future releases could incorporate a completely
 * different solution, such as a Machine Learning model, recognising
 * numbers and letters.
 * 
 * @param params structure holding all parameters of
 * card-shaped objects
 * @param rank boolean, can be used to select detection of
 * rank (== true) or suit (== false). Suit detection is currently
 * not implemented. 
 * 
 */
void DetectCard::template_matching(Card_params &params, bool rank){
    carddiscriminator.template_matching(params, rank);
}

/**
 * @brief Main loop used by Processing Thread.
 * Here all the processing is performed whenever a new frame
 * has been captured by the camera. Loop ends by calling 
 * the callback of the obe
 * 
 */
void DetectCard::processingThreadLoop(){
    while(isProcessing){
        if(newFrame){
            /* Processing loop busy */
            busy = true;
            // std::cout << "Frames dropped: " << frame_counter << std::endl;
            /* First preprocess the entire frame */
            cv::Mat processed_image = preprocess_image(currentFrame);
            if (!err_frame){
                /* Next find cards in the frame
                 * Need to Show frame after find_cards()
                 * cv::imshow converts image to 3-channel
                 * find_cards() requires single monochrome channel */
                Card_params card_params = find_cards(processed_image);
                /* Find the cards rank ROI (region of interest) */
                preprocess_cards(card_params, processed_image);
                /* Take isolated ranks and match them with the template cards */
                template_matching(card_params);

                card_params.currentFrame = currentFrame;
                /* Take Card Names and respective Card Positions 
                 * and pass it to the GamePlay class */
                if (processingCallback){
                    AcePlaysUtils callbackData;
                    callbackData.cardParams = card_params;
                    processingCallback->nextCallback(callbackData);
                }
            }

            /* Processing finished */
            newFrame = false;
            busy = false;
            err_frame = false;
            frame_counter = 0;
            
            /* The following cv::waitKey() statement is needed 
            for cv::imshow to work. Can be removed if using a different
            frame showing package, such as Qt GUI */
            int key = cv::waitKey(1);
            if (key == 27/*ESC*/){break;}
        }
    }
}

/**
 * @brief Implement the nextCallback virtual method to pass
 * information down the pipeline
 * 
 * @param callbackData an AcePlaysUtils callback object
 */
void DetectCard::nextCallback(AcePlaysUtils &callbackData){
    /* Count dropped frames when processing loop is busy */
    frame_counter++;
    newFrame = true;
    currentFrame = callbackData.nextFrame;
}

/**
 * @brief Register a Callback for the DetectCard Class
 * 
 * @param cb is a CallbackLinker class pointer
 */
void DetectCard::registerCallback(CallbackLinker* cb){
    processingCallback = cb;
}

/**
 * @brief Unregister existing callback
 * 
 */
void DetectCard::unregisterCallback(){
    processingCallback = nullptr;
}

/**
 * @brief Method call to start the Processing Thread
 * 
 */
void DetectCard::startProcessing(){
    isProcessing = true;
    /* Start Thread */
    procThread = std::thread(&DetectCard::processingThreadLoop, this);
}

/**
 * @brief Method call to stop the Processing Thread
 * 
 */
void DetectCard::stopProcessing(){
    // isProcessing = false;
    /* If not using Qt or dont have anything running in main() 
      use .join() method to keep the procThread running */
    procThread.join();
}

/**
 * @brief Construct a new Detect Card:: Detect Card object
 * 
 * @param folder_path get folder path of template cards and 
 * call CardDiscriminator constructor to load all templates
 */
DetectCard::DetectCard(cv::String folder_path):carddiscriminator(folder_path)
{
}

/**
 * @brief Destroy the Detect Card:: Detect Card object
 * 
 */
DetectCard::~DetectCard()
{
}
