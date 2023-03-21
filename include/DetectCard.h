#ifndef DETECT_CARD_H
#define DETECT_CARD_H
#include <opencv2/core.hpp>
#include "CallbackLinker.h"
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

/**
 * @brief Class to load and store the template cards
 * from a folder path
 * 
 */
class CardTemplate
{
private:
    /* Load template cards once */
    const std::vector<cv::Mat> template_cards;

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
    CardTemplate(cv::String folder) : template_cards(init(folder)), num_template_cards((std::uint8_t)template_cards.size()) {};
    
    /**
     * @brief Init function to parse the folder path and read
     * the template card images.
     * 
     * @param folder is a string to the folder path 
     * @return std::vector<cv::Mat> 
     */
    static std::vector<cv::Mat> init(cv::String folder)
    {
	   std::vector<cv::String> filenames;
       std::vector<cv::Mat> result;
       cv::glob(folder, filenames);

       /**
        * @brief read and store each image as single
        * channel GRAYSCALE
        */
       for (size_t i = 0; i < filenames.size(); i++){
            result.push_back(cv::imread(filenames[i], cv::IMREAD_GRAYSCALE));
       }
        return result;
    }

    /**
     * @brief Get the Card object
     * 
     * @param index index of card in template_cards object
     * @return cv::Mat single template card at index location
     */
    cv::Mat getCard(size_t index){
        if(index > template_cards.size() || index < 0){
            std::cout << "Error: Index Invalid" << endl;
            return cv::Mat{};
        }
		cv::Mat card = template_cards.at(index);
        return card;
    }    
};


class DetectCard
{
private:
    bool isProcessing = false;
    bool newFrame = false;
    cv::Mat currentFrame;
    std::thread procThread;
    void processingThreadLoop();
    CallbackLinker* processingCallback = nullptr;

    cv::Mat preprocess_image(cv::Mat image);
    struct Card_params DetectCard::find_cards(cv::Mat image);
    cv::Mat DetectCard::flatten_card(Query_card qCard, cv::Mat image);
    cv::Mat DetectCard::preprocess_card(cv::Mat image, struct Card_params Card_params);

public:
    DetectCard(/* args */);
    ~DetectCard();
    void registerCallback(CallbackLinker* cb);
    void unregisterCallback();

    /* Output, match template and store cards detected */
    void DetectCard::template_matching(cv::Mat roi, CardTemplate card_templates, bool rank=true);

    /* Spawn thread */
    void startProcessing();
};




#endif /* DETECT_CARD_H */