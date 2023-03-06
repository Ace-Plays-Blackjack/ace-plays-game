#ifndef CALLBACK_LINKER_H 
#define CALLBACK_LINKER_H

#include <opencv2/core.hpp>

class CallbackLinker
{
public:
    CallbackLinker(/* args */);
    ~CallbackLinker();

    virtual void passFrame(cv::Mat nextFrame) = 0;

};

CallbackLinker::CallbackLinker(/* args */)
{
}

CallbackLinker::~CallbackLinker()
{
}



#endif /*CALLBACK_LINKER_H*/