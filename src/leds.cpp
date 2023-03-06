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
	decision = STAND;
	return;
};
bool led_flasher::changeled(bool on){
	if (on) {
	   std::cout << "flash ON" << std::endl;
	}
	else {
	   std::cout << "flash OFF" << std::endl;
	}
	return true;
};
bool led_flasher::mainthread(){
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
		changeled(ledValue);
		ledValue = !ledValue;
		switch (decision) {
			case HIT:
				gpio(18, ledValue);
				break;
			case STAND:
				gpio(22, ledValue);
				break;
			case SPLIT:
				gpio(17, ledValue);
				break;
			case DOUBLE:
				gpio(27, ledValue);
				break;
			case UNKNOWN:
				break;
		}
		usleep (100000);
		i++;
	}
	gpioWrite(17, PI_HIGH);
	gpioWrite(18, PI_HIGH);
	gpioWrite(22, PI_HIGH);
	gpioWrite(27, PI_HIGH);
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


