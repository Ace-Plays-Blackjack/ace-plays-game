#include "StrategyEngine.h"

StrategyEngine::StrategyEngine() {

}

decisions StrategyEngine::getchoice(int dealercard, std::vector<int> playercards) {
	
	int numofcards = (int)playercards.size();
	int i = 0;
	int playertotal = 0;
	bool soft = false;
	bool split = false;
	if (numofcards == 2) {
		if (playercards[0] == playercards[1]) {
			split = true;
		}
	}
	while (i < numofcards) {
		if ((playercards[i] == 11) && (playertotal + playercards[i] <= 21)) {
			soft = true;
		}
		playertotal = playercards[i] + playertotal;
		i++;
		if ((playertotal > 21) && (soft)) {
			playertotal = playertotal - 10;
			soft = false;
		}
		std::cout << playertotal << std::endl;
		if (playertotal > 21) {
			std::cout << "You Lose!" << std::endl;
			return LOSE;
		}
	}
	
	if ((playertotal == 11) && (dealercard < 11) && (numofcards == 2)) {
		std::cout << "DOUBLE!" << std::endl;
		return DOUBLE;
	}
	if ((playertotal == 10) && (dealercard < 10) && (numofcards == 2)) {
		std::cout << "DOUBLE!" << std::endl;
		return DOUBLE;
	}
	if ((playertotal == 9) && ((dealercard == 3) || (dealercard == 4) || (dealercard == 5) || (dealercard = 6))) {
		std::cout << "DOUBLE!" << std::endl;
		return DOUBLE;
	}
	if ((soft) && (numofcards == 2)) {
		if (((dealercard == 5) || (dealercard == 6)) && (playertotal > 12) && (playertotal < 19)) {
			std::cout << "DOUBLE!" << std::endl;
			return DOUBLE;
		}
		if ((dealercard == 4) && (playertotal >= 15) && (playertotal < 19)) {
			std::cout << "DOUBLE!" << std::endl;
			return DOUBLE;
		}
		if ((dealercard == 3) && (playertotal >= 17) && (playertotal <= 18)) {
			std::cout << "DOUBLE!" << std::endl;
			return DOUBLE;
		}
	} 
	if (split) {
		if (((playertotal == 4) || (playertotal == 6)) && (dealercard <= 7)) {
			std::cout << "SPLIT!" << std::endl;
			return SPLIT;
		}
		if ((playertotal == 8) && ((dealercard == 5) || (dealercard == 6))) {
			std::cout << "SPLIT!" << std::endl;
			return SPLIT;
		}
		if ((playertotal == 12) && (dealercard <= 6)) {
			std::cout << "SPLIT!" << std::endl;
			return SPLIT;
		}
		if ((playertotal == 14) && (dealercard <= 7)) {
			std::cout << "SPLIT!" << std::endl;
			return SPLIT;
		}
		if ((playertotal == 16) && (dealercard <= 9)) {
			std::cout << "SPLIT!" << std::endl;
			return SPLIT;
		}
		if ((playertotal == 18) && (dealercard <= 9) && (dealercard != 7)) {
			std::cout << "SPLIT!" << std::endl;
			return SPLIT;
		}
	}
	if (playertotal <= 11) {
		std::cout << "HIT!" << std::endl;
		return HIT;
	}
	if (soft) {
		if (playertotal <= 17) {
			std::cout << "HIT!" << std::endl;
			return HIT;
		}
		if ((playertotal == 18) && (dealercard > 8)) {
			std::cout << "HIT!" << std::endl;
			return HIT;
		}
	}
	if ((playertotal > 11) && (playertotal < 17) && (dealercard > 6)) {
		std::cout << "HIT!" << std::endl;
		return HIT;
	}
	if ((playertotal == 12) && ((dealercard == 2) || (dealercard == 3))) {
		std::cout << "HIT!" << std::endl;
		return HIT;
	}
	std::cout << "STAND!" << std::endl;
	return STAND;
}