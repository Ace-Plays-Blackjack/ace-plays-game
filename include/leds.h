#ifndef LEDS_H
#define LEDS_H

enum decisions {
    HIT,
    STAND,
    SPLIT,
    DOUBLE,
    UNKNOWN,
    STOP
};


class led_flasher {
    public: 
    led_flasher();
    bool changeled(bool on);
    bool mainthread(decisions (*gd)());
    bool gpio(int led);

};

#endif //LEDS_H

