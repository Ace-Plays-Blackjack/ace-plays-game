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
	bool ledValue = false;
	int i = 0;
	while (i < 100) {
		ledValue = !ledValue;
		switch ((*gd)()) {
			case HIT:
				gpio(12, ledValue);
				break;
			case STAND:
				gpio(19, ledValue);
				break;
			case SPLIT:
				gpio(18, ledValue);
				break;
			case DOUBLE:
				gpio(13, ledValue);
				break;
			case UNKNOWN:
				break;
		}
		usleep (100000);
		i++;
	}
	gpioWrite(12, PI_HIGH);
	gpioWrite(13, PI_HIGH);
	gpioWrite(18, PI_HIGH);
	gpioWrite(19, PI_HIGH);
	gpioTerminate();
	return true;
};

bool led_flasher::gpio(int led, bool on) {
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
   if (on) {
	gpioWrite(GPIO, PI_LOW);
	v = gpioGetMode(GPIO);

	v = gpioRead(GPIO);
   }
   else {
	gpioWrite(GPIO, PI_HIGH);
	gpioDelay(1); /* 1 micro delay to let GPIO reach level reliably */
	v = gpioRead(GPIO);
	}
	return true;
}


