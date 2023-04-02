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
    cv::Mat flatten_card(Query_card qCard, cv::Mat &image);
    std::vector<cv::Mat> preprocess_cards(cv::Mat &image, Card_params Card_params);
    /* Output, match template and store cards detected */
    void template_matching(const std::vector<cv::Mat> &roi, bool rank=true);

public:
    DetectCard(cv::String folder_path);
    ~DetectCard();
    void registerCallback(CallbackLinker* cb);
    void unregisterCallback();
    void passFrame(cv::Mat &nextFrame);

    /* Spawn thread */
    void startProcessing();
};




#endif /* DETECT_CARD_H */