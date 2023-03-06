class led_flasher {
    public: 
    led_flasher();
    bool changeled(bool on);
    bool mainthread();
    bool gpio(int led, bool on);
    enum decisions {
        HIT,
        STAND,
        SPLIT,
        DOUBLE,
        UNKNOWN
    } decision;
};