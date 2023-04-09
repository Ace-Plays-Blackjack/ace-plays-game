#ifndef STRATEGY_ENGINE_H
#define STRATEGY_ENGINE_H

#include <iostream>
#include <vector>
#include "Card.h"

class StrategyEngine {
public:
	StrategyEngine();
	decisions getchoice(int dealercard, std::vector<int> playecards);
};

#endif //STRATEGY_ENGINE_H