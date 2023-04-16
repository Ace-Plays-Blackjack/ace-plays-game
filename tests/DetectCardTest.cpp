#include <gtest/gtest.h>
#include <opencv2/opencv.hpp>
#include "DetectCard.h"
#include "Card.h"

TEST(DetectCardTest, FindCards) {

    cv::Mat img = cv::imread("../tests/test_image.jpg");
    cv::String image_path = "../TemplateImages/";
    DetectCard detectCard(image_path);
    
    cv::Mat processed_img = detectCard.preprocess_image(img);

    Card_params card_params = detectCard.find_cards(processed_img);

    EXPECT_GT(card_params.num_of_cards, 1);
}

TEST(DetectCardTest, FlattenCard) {

    cv::Mat img = cv::imread("../tests/test_image.jpg");

    cv::String image_path = "../TemplateImages/";
    DetectCard detectCard(image_path);
    cv::Mat processed_img = detectCard.preprocess_image(img);
    Card_params card_params = detectCard.find_cards(processed_img);
    
    for (int i = 0; i < card_params.num_of_cards; i++) {

        qCard qcard;
        qcard.corner_pts = card_params.card_approxs[i];
        qcard.card_size = cv::Size(350, 500);

        cv::Mat flat_card = detectCard.flatten_card(qcard, img);

        EXPECT_EQ(flat_card.cols, 350);
        EXPECT_EQ(flat_card.rows, 500);
    }
}
