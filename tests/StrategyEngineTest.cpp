/*
Copyright 2023 Georgios Titas
Copyright 2023 Alexander Douglas
Copyright 2023 Jijo Varghese
Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the “Software”), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

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

