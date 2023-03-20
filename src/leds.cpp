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



led_flasher::led_flasher(){
	std::cout << "construct led flasher" << std::endl;
	return;
};

bool led_flasher::mainthread(decisions (*gd)()) {
	std::cout << "starting flasher" << std::endl;
	int status;   
	status = gpioInitialise();

    if (status < 0)
	{
		fprintf(stderr, "pigpio initialisation failed.\n");
		return 1;
	}
	bool done = false;
	gpio(12);
	gpio(13);
	gpio(18);
	gpio(19);
	
	while (!done) {
		switch ((*gd)()) {
		case HIT:
			gpioWrite(12, PI_LOW);
			gpioWrite(13, PI_HIGH);
			gpioWrite(18, PI_HIGH);
			gpioWrite(19, PI_HIGH);
			break;
		case STAND:
			gpioWrite(12, PI_HIGH);
			gpioWrite(13, PI_HIGH);
			gpioWrite(18, PI_HIGH);
			gpioWrite(19, PI_LOW);
			break;
		case SPLIT:
			gpioWrite(12, PI_HIGH);
			gpioWrite(13, PI_HIGH);
			gpioWrite(18, PI_LOW);
			gpioWrite(19, PI_HIGH);
			break;
		case DOUBLE:
			gpioWrite(12, PI_HIGH);
			gpioWrite(13, PI_LOW);
			gpioWrite(18, PI_HIGH);
			gpioWrite(19, PI_HIGH);
			break;
		case UNKNOWN:
			break;
		case STOP:
			gpioWrite(12, PI_HIGH);
			gpioWrite(13, PI_HIGH);
			gpioWrite(18, PI_HIGH);
			gpioWrite(19, PI_HIGH);
			gpioTerminate();
			done = true;
			break;
	}
	}
	
	gpioTerminate();
	return true;
};

bool led_flasher::gpio(int led) {
	int v;
	int GPIO = led;
   gpioSetMode(GPIO, PI_INPUT);
   v = gpioGetMode(GPIO);

   gpioSetPullUpDown(GPIO, PI_PUD_UP);
   gpioDelay(1); /* 1 micro delay to let GPIO reach level reliably */
   v = gpioRead(GPIO);

   gpioSetPullUpDown(GPIO, PI_PUD_DOWN);
   gpioDelay(1); /* 1 micro delay to let GPIO reach level reliably */
   v = gpioRead(GPIO);
	return true;
}