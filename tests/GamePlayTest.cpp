/*
Copyright 2023 Georgios Titas
Copyright 2023 Alexander Douglas
Copyright 2023 Jijo Varghese
Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the “Software”), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#include <gtest/gtest.h>
#include "GamePlay.h"

class GamePlayTest : public ::testing::Test {

};

TEST(GamePlayTest, ConvertStr2Int) {
  std::vector<std::string> cards = {"Three", "Seven", "Ace", "Five"};
  std::vector<int> expected_values = {3, 7, 11, 5};
  GamePlay game_play(0.0, 0.0);  
  std::vector<int> values = game_play.convertStr2Int(cards);
  EXPECT_EQ(values, expected_values);
}

TEST(GamePlayTest, WhosHandTest) {
    GamePlay game(640, 480);
    cv::Point_<int> dealer_card_pt(320, 120);
    cv::Point_<int> player_card_pt(320, 360);
    game.whosHand(dealer_card_pt);
    game.whosHand(player_card_pt);
    EXPECT_EQ(game.num_dealer_cards, 1);
    EXPECT_EQ(game.whos_hand[1], true);
}

TEST(GamePlayTest, GameResetTest) {
    GamePlay game(640, 480);
    game.num_dealer_cards = 2;
    game.num_player_cards = 2;
    game.dealersHand.cards = {8, 5};
    game.playersHand.cards = {9, 4};
    game.game_reset();
    
    EXPECT_EQ(game.gameStarted, false);
    EXPECT_EQ(game.total_cards, 0);
    EXPECT_EQ(game.num_dealer_cards, 0);
    EXPECT_EQ(game.num_player_cards, 0);
    EXPECT_EQ(game.dealersHand.cards.size(), 0);
    EXPECT_EQ(game.playersHand.cards.size(), 0);
}
