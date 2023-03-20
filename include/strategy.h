#ifndef STRATEGY_H
#define STRATEGY_H

#include "leds.h"
#include <vector>

class decisionmaker {
	public:
	decisionmaker();
	decisions getchoice(int dealercard, std::vector<int> playecards);
};

#endif //STRATEGY_H