#ifndef LEDS_H
#define LEDS_H
#include "Card.h"

class ToggleLED {
public: 
    ToggleLED();
    ~ToggleLED();
    bool flashled(decisions choice);
private:
    void gpio(int led);

};

#endif //LEDS_H