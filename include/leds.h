#ifndef LEDS_H
#define LEDS_H

enum decisions {
    HIT,
    STAND,
    SPLIT,
    DOUBLE,
    UNKNOWN,
    LOSE,
    STOP
};


class ToggleLED {
public: 
    ToggleLED();
    ~ToggleLED();
    bool flashled(decisions choice);
private:
    bool gpio(int led);
};

#endif //LEDS_H
