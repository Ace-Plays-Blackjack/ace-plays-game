#ifndef CARDS_H
#define CARDS_H

#include <opencv2/core.hpp>
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


struct Card_params{
    int err = 0;
    int num_of_cards = 0;
    std::vector<int> contour_is_card_idx;
    std::vector<std::vector<cv::Point>> contours;
    std::vector<std::vector<cv::Point>> card_approxs;
    std::vector<cv::RotatedRect> rotatedbox;
    std::vector<std::vector<cv::Point2f>> rotatedbox_pts; 
};

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

struct TemplateImages{
    /* Images of loaded template cards */
    std::vector<cv::Mat> template_cards;
    /* Names of loaded template cards */
    std::vector<cv::String> names;
};

/**
 * @brief Class to load and store the template cards
 * from a folder path
 * 
 */
class CardTemplate
{
private:
    /* Load template ranks once */
    const TemplateImages rank_images;
    /* Not implemented yet */
    const TemplateImages suit_images;

public:
    /* Keep track on number of template cards stored */
    const std::uint8_t num_template_cards;

    /**
     * @brief Construct a new Card Template object.
     * Template cards are defined as constant. The following
     * procedure (initialiser list) is required to set a const
     * in runtime.
     * 
     * Similarly set the num_template_cards variable.
     * 
     * @param folder is a string to the folder path 
     */
    CardTemplate(cv::String folder) : rank_images(init(folder, false)), suit_images(init(folder, true)), num_template_cards((std::uint8_t) rank_images.template_cards.size()) {};
    
    /**
     * @brief Init function to parse the folder path and read
     * the template card images.
     * 
     * @param folder is a string to the folder path 
     * @param suit_flag boolean to distinguish between loading 
     * of ranks or suit images
     * @return TemplateImages 
     */
    static TemplateImages init(cv::String folder, bool suit_flag)
    {
        TemplateImages temp;
        cv::String filename;
        std::vector<cv::Mat> result;
        std::vector<cv::String> card_names;

        if(suit_flag){
            card_names = {"Club","Heart","Diamond","Spade"};
        }
        else{
            card_names = {"Ace","Two","Three","Four","Five","Six","Seven","Eight","Nine","Ten","Jack","Queen","King"};
        }

        for (size_t i = 0; i < card_names.size(); i++){
            filename = folder + card_names[i] + ".jpg";
            result.push_back(cv::imread(filename, cv::IMREAD_GRAYSCALE));
        }

        temp.template_cards = result;
        temp.names = card_names;        
        return temp;
    }

    /**
     * @brief Get the Card object
     * 
     * @param index index of card in rank_images object
     * @return cv::Mat single template card at index location
     */
    cv::Mat getCard(size_t index){
        if(index > rank_images.template_cards.size() || index < 0){
            std::cout << "Error: Index Invalid" << std::endl;
            return cv::Mat{};
        }
        return rank_images.template_cards.at(index);
    }

    /**
     * @brief Get the Card name
     * 
     * @param index index of card in rank_images object
     * @return cv::String name of detected card
     */
    cv::String getCardRank(size_t index){
        if(index > rank_images.names.size() || index < 0){
            std::cout << "Error: Index Invalid" << std::endl;
            return "None";
        }
        return rank_images.names.at(index);
    }
};



#endif /* CARDS_H */