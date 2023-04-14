/*
Copyright 2023 Georgios Titas
Copyright 2023 Alexander Douglas
Copyright 2023 Jijo Varghese
Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the “Software”), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#ifndef CARDDISCRIMINATOR
#define CARDDISCRIMINATOR


#include "Card.h"

/**
 * @brief Simple structure to hold loaded images
 * and their respective names
 * 
 */
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
    const int num_template_cards;

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
    CardTemplate(cv::String folder) : rank_images(init(folder, false)), suit_images(init(folder, true)), num_template_cards((int)rank_images.template_cards.size()) {};
    
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
    cv::Mat getCard(int index){
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
    cv::String getCardRank(int index){
        if(index > rank_images.names.size() || index < 0){
            std::cout << "Error: Index Invalid" << std::endl;
            return "None";
        }
        return rank_images.names.at(index);
    }
};

/**
 * @brief Class to implement detection method from 
 * Card Rank ROIs. Can be adapted for other solutions
 * such as machine learning
 * 
 */
class CardDiscriminator {
	public:
		CardDiscriminator(cv::String folder_path);
		void template_matching(Card_params &Card_params, bool rank);
		
	private:
	    CardTemplate cardTemplates;
	
};

#endif //CARD_DISCRIMINATOR_H
