#include "CardDiscriminator.h"
#include "Card.h"

CardDiscriminator::CardDiscriminator(cv::String folder_path): cardTemplates(folder_path) {
	
}

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
            // result = card_templates.getCard(matching_card_idx);
            result_name.push_back(cardTemplates.getCardRank(matching_card_idx));
        }
    }
    // for (int i = 0; i < result_name.size(); i++){
    //     std::cout << "Card " + std::to_string(i) + ":" << result_name[i] << std::endl;
    // }

    params.card_names = result_name;
}
