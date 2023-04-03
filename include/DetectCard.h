#ifndef DETECT_CARD_H
#define DETECT_CARD_H

#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <thread>

#include "Card.h"
#include "CallbackLinker.h"

class DetectCard : public CallbackLinker
{
private:
    CardTemplate cardTemplates;
    bool isProcessing = false;
    bool busy = false;
    bool newFrame = false;
    int frame_counter = 0;
    cv::Mat currentFrame;

    std::thread procThread;
    void processingThreadLoop();
    CallbackLinker* processingCallback = nullptr;

    cv::Mat preprocess_image(cv::Mat &image);
    Card_params find_cards(cv::Mat &image);
    cv::Mat flatten_card(qCard &qCard, cv::Mat &image);
    void preprocess_cards(Card_params &Card_params, cv::Mat &image);

    /* Output, match template and store cards detected */
    void template_matching(Card_params &Card_params, bool rank=true);

public:
    DetectCard(cv::String folder_path);
    ~DetectCard();
    void registerCallback(CallbackLinker* cb);
    void unregisterCallback();
    void nextCallback(cv::Mat &nextFrame);
    void nextCallback(Card_params &Card_params){};

    /* Spawn thread */
    void startProcessing();
};




#endif /* DETECT_CARD_H */