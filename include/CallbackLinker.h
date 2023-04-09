#ifndef CALLBACK_LINKER_H 
#define CALLBACK_LINKER_H

#include "Card.h"

/**
 * @brief Generic abstract Callback Interface class
 * 
 */
class CallbackLinker
{
public:
    /**
     * @brief Abstract method
     * @details Implement method in a Derived class
     * 
     * @param nextFrame a cv::Mat type object
     */
    virtual void nextCallback(cv::Mat &nextFrame) = 0;
    // virtual void nextCallback(AcePlaysUtils &callbackData) = 0;
};


#endif /*CALLBACK_LINKER_H*/