/*
Copyright 2023 Georgios Titas
Copyright 2023 Alexander Douglas
Copyright 2023 Jijo Varghese
Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the “Software”), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#include "CardDiscriminator.h"
#include "Card.h"

/**
 * @brief Construct a new Card Discriminator:: Card Discriminator object
 * 
 * @param folder_path get folder path of template cards and 
 * call cardTemplates constructor to load all templates
 */
CardDiscriminator::CardDiscriminator(cv::String folder_path): cardTemplates(folder_path) {
	
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
void CardDiscriminator::template_matching(Card_params &params, bool rank){

    std::vector<cv::String> result_name;
    std::vector<cv::Mat> roi = params.rank_rois;

    for (int i = 0; i < roi.size(); i++)
    {
        if (roi[i].rows == RANK_HEIGHT && roi[i].cols == RANK_WIDTH){
            /* Clone roi image */
            cv::Mat result(cv::Size(roi[i].rows, roi[i].cols), CV_8UC1);
            /* Initialise as a high number, i.e. all pixels are non-zero */
            int prev_count = roi[i].rows * roi[i].cols;
            int num_non_zero, matching_card_idx;

            for (int k = 0; k < cardTemplates.num_template_cards; k++)
            {
                try{
                    /* absdiff may crash if roi is empty */
                    cv::absdiff(roi[i], cardTemplates.getCard(k), result);
                }
                catch (cv::Exception& e)
                {
                    const char* err_msg = e.what();
                    // std::cout << "Exception caught: " << __FUNCTION__ << " cv::absdiff:\n" << err_msg << std::endl;
                    break;
                }
                num_non_zero = cv::countNonZero(result);
                if (num_non_zero < prev_count){
                    prev_count = num_non_zero;
                    matching_card_idx = k;
                }
            }
            result_name.push_back(cardTemplates.getCardRank(matching_card_idx));
        }
    }

    params.card_names = result_name;
}
