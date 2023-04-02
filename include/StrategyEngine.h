#ifndef STRATEGY_ENGINE_H
#define STRATEGY_ENGINE_H

#include "leds.h"
#include <vector>

class StrategyEngine {
	public:
	StrategyEngine();
	decisions getchoice(int dealercard, std::vector<int> playecards);
};

#endif //STRATEGY_ENGINE_H