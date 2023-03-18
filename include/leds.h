#ifndef LEDS_H
#define LEDS_H

enum decisions {
    HIT,
    STAND,
    SPLIT,
    DOUBLE,
    UNKNOWN
};


class led_flasher {
    public: 
    led_flasher();
    bool changeled(bool on);
    bool mainthread(decisions (*gd)());
    bool gpio(int led, bool on);

};

#endif //LEDS_H
