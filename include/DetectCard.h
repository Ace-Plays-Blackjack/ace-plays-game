#ifndef DETECT_CARD_H
#define DETECT_CARD_H

#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>  // cv::Canny()
#include "Card.h"
#include "CallbackLinker.h"

class DetectCard
{
private:
    bool isProcessing = false;
    bool newFrame = false;
    cv::Mat currentFrame;
    std::thread procThread;
    void processingThreadLoop();
    CallbackLinker* processingCallback = nullptr;

    cv::Mat preprocess_image(cv::Mat &image);
    struct Card_params DetectCard::find_cards(cv::Mat &image);
    cv::Mat DetectCard::flatten_card(Query_card qCard, cv::Mat &image);
    std::vector<cv::Mat> DetectCard::preprocess_card(cv::Mat &image, struct Card_params Card_params);

public:
    DetectCard(/* args */);
    ~DetectCard();
    void registerCallback(CallbackLinker* cb);
    void unregisterCallback();

    /* Output, match template and store cards detected */
    void DetectCard::template_matching(std::vector<cv::Mat> &roi, CardTemplate card_templates, bool rank=true);

    /* Spawn thread */
    void startProcessing();
};




#endif /* DETECT_CARD_H */