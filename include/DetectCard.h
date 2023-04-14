/*
Copyright 2023 Georgios Titas
Copyright 2023 Alexander Douglas
Copyright 2023 Jijo Varghese
Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the “Software”), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#ifndef DETECT_CARD_H
#define DETECT_CARD_H

#include <thread>

#include "Card.h"
#include "CallbackLinker.h"
#include "CardDiscriminator.h"

/**
 * @brief Class for the detection of cards. Runs the processing pipeline.
 * 
 */
class DetectCard : public CallbackLinker
{
private:
    bool isProcessing = false;
    bool busy = false;
    bool newFrame = false;
    bool err_frame = false;
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
    CardDiscriminator carddiscriminator;

public:
    DetectCard(cv::String folder_path);
    ~DetectCard();
    void registerCallback(CallbackLinker* cb);
    void unregisterCallback();
    void nextCallback(AcePlaysUtils &callbackData);

    /* Spawn thread */
    void startProcessing();
    void stopProcessing();
};




#endif /* DETECT_CARD_H */