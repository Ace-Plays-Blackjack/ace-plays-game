#ifndef CALLBACK_LINKER_H 
#define CALLBACK_LINKER_H

#include <opencv2/core.hpp>

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
     * @param nextFrame a cv::Mat type variable
     */
    virtual void passFrame(cv::Mat &nextFrame) = 0;
};


#endif /*CALLBACK_LINKER_H*/