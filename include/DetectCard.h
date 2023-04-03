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
    cv::Mat flatten_card(DetectedCard &qCard, cv::Mat &image);
    void preprocess_cards(cv::Mat &image, Card_params &Card_params);

    /* Output, match template and store cards detected */
    std::vector<cv::String> template_matching(const Card_params &Card_params, bool rank=true);

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