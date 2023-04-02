#include <unistd.h>
#include <iostream>
#include "leds.h"

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>

#include "pigpio.h"

#define HIT_PIN 12
#define STAND_PIN 19
#define SPLIT_PIN 18
#define DOUBLE_PIN 13

ToggleLED::ToggleLED(){
	std::cout << "construct led flasher" << std::endl;
	int status;   
	status = gpioInitialise();

    if (status < 0)
	{
		fprintf(stderr, "pigpio initialisation failed.\n");
	}
	gpio(HIT_PIN);
	gpio(STAND_PIN);
	gpio(SPLIT_PIN);
	gpio(DOUBLE_PIN);
}

bool ToggleLED::flashled(decisions choice) {
	
	switch (choice) {
		case HIT:
			gpioWrite(HIT_PIN, PI_LOW);
			gpioWrite(DOUBLE_PIN, PI_HIGH);
			gpioWrite(SPLIT_PIN, PI_HIGH);
			gpioWrite(STAND_PIN, PI_HIGH);
			break;
		case STAND:
			gpioWrite(HIT_PIN, PI_HIGH);
			gpioWrite(DOUBLE_PIN, PI_HIGH);
			gpioWrite(SPLIT_PIN, PI_HIGH);
			gpioWrite(STAND_PIN, PI_LOW);
			break;
		case SPLIT:
			gpioWrite(HIT_PIN, PI_HIGH);
			gpioWrite(DOUBLE_PIN, PI_HIGH);
			gpioWrite(SPLIT_PIN, PI_LOW);
			gpioWrite(STAND_PIN, PI_HIGH);
			break;
		case DOUBLE:
			gpioWrite(HIT_PIN, PI_HIGH);
			gpioWrite(DOUBLE_PIN, PI_LOW);
			gpioWrite(SPLIT_PIN, PI_HIGH);
			gpioWrite(STAND_PIN, PI_HIGH);
			break;
		case UNKNOWN:
			break;
		case STOP:
			gpioWrite(HIT_PIN, PI_HIGH);
			gpioWrite(DOUBLE_PIN, PI_HIGH);
			gpioWrite(SPLIT_PIN, PI_HIGH);
			gpioWrite(STAND_PIN, PI_HIGH);
			gpioTerminate();
			break;
	}
	
	return true;
}

void ToggleLED::gpio(int led) {
	int GPIO = led;
	gpioSetMode(GPIO, PI_OUTPUT);
	gpioSetPullUpDown(GPIO, PI_PUD_UP);
	gpioDelay(1); /* 1 micro delay to let GPIO reach level reliably */
}

ToggleLED::~ToggleLED() {
	gpioTerminate();
}