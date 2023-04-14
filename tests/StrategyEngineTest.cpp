#include <gtest/gtest.h>
#include "StrategyEngine.h"


StrategyEngine strategy;

TEST(StrategyTest, Hit) {
  std::vector<int> cards = {7, 8}; // player cards
  decisions result = strategy.getchoice(7, cards); // dealer card
  EXPECT_EQ(result, HIT);
}

TEST(StrategyTest, Double) {
  std::vector<int> cards = {7, 4}; 
  decisions result = strategy.getchoice(5, cards); 
  EXPECT_EQ(result, DOUBLE);
}

TEST(StrategyTest, Split) {
  std::vector<int> cards = {6, 6}; 
  decisions result = strategy.getchoice(4, cards); 
  EXPECT_EQ(result, SPLIT);
}

TEST(StrategyTest, Stand) {
  std::vector<int> cards = {10, 7}; 
  decisions result = strategy.getchoice(2, cards); 
  EXPECT_EQ(result, STAND);
}

TEST(StrategyTest, Lose) {
  std::vector<int> cards = {10, 9, 3}; 
  decisions result = strategy.getchoice(5, cards); 
  EXPECT_EQ(result, LOSE);
}

