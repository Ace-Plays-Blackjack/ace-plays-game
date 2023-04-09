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
     * @param callbackData an AcePlaysUtils type object to pass all
     * relevant data down the AcePlays pipeline
     */
    virtual void nextCallback(AcePlaysUtils &callbackData) = 0;
};


#endif /*CALLBACK_LINKER_H*/