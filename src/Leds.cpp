/*
Copyright 2023 Georgios Titas
Copyright 2023 Alexander Douglas
Copyright 2023 Jijo Varghese
Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the “Software”), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#include "Leds.h"
#include "pigpio.h"


#define HIT_PIN 12
#define STAND_PIN 19
#define SPLIT_PIN 18
#define DOUBLE_PIN 13

ToggleLED::ToggleLED(){
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
