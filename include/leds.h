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
    ~led_flasher();
    bool flashled(decisions choice);
    private:
    bool changeled(bool on);
    bool gpio(int led);

};

#endif //LEDS_H

